#include "../include/panels/transfer.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

Transfer::Transfer(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   wxTextValidator* numeric_validator = new wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC);
   wxTextValidator* empty_validator = new wxTextValidator(wxFILTER_EMPTY);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   to->SetValidator(*empty_validator);

   amount->SetValidator(*numeric_validator);

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
   const auto from_value = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto to_value = to->GetValue().ToStdString();
   const auto amount_value = amount->GetValue().ToStdString();
   const auto asset_value = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();
   const auto memo_value = memo->GetValue().ToStdString();
   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(to_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is invalid"));
      to->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "transfer " + from_value + " " + to_value + " " + amount_value + " " + asset_value +
            " \"" + memo_value + "\" " + broadcast_value;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(from_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(from_value, to_value, amount_value, asset_value,
               memo_value, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm transfer?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(from_value, to_value, amount_value, asset_value,
                     memo_value, true);
               p_GWallet->DoAssets(from_value);
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
   gwallet->panels.p_commands->notebook->AddPage(this, "Transfer response", true);
}
