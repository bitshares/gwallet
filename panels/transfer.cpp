#include "../include/panels/transfer.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

Transfer::Transfer(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   //to->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   asset->Append(p_GWallet->strings.assets);
   asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Transfer::OnOk));
   Connect(XRCID("to"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(Transfer::OnSearchAccount), NULL, this);

   SetScrollRate(1,1);
}

void Transfer::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *to);
}

void Transfer::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _from = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto _to = to->GetValue().ToStdString();
   const auto _amount = amount->GetValue().ToStdString();
   const auto _asset = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();
   const auto _memo = memo->GetValue().ToStdString();
   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(_to);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is invalid"));
      to->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "transfer " + _from + " " + _to + " " + _amount + " " + _asset +
            " \"" + _memo + "\" " + _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(_from);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(_from, _to, _amount, _asset,
               _memo, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm transfer?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(_from, _to, _amount, _asset,
                     _memo, true);
               p_GWallet->DoAssets(_from);
            }
         }
         response = result_obj;
         new TransferResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

TransferResponse::TransferResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Transfer response"), true);
}
