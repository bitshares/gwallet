#include "../include/panels/extendhtlc.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

ExtendHtlc::ExtendHtlc(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   issuer->Append(p_GWallet->strings.accounts);
   issuer->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExtendHtlc::OnOk));
   Connect(XRCID("htlc_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(ExtendHtlc::OnSearchID), NULL, this);

   SetScrollRate(1,1);
}

void ExtendHtlc::OnSearchID(wxCommandEvent& event)
{
   //p_GWallet->DoSearchID(event.GetString(), *htlc_id);
}

void ExtendHtlc::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _issuer = p_GWallet->strings.accounts[issuer->GetCurrentSelection()].ToStdString();
   const auto _htlc_id = htlc_id->GetValue().ToStdString();
   const auto _seconds_to_add = minutes_to_add->GetValue() * 60;

   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "htlc_extend " + _htlc_id + " " + _issuer + " " + to_string(_seconds_to_add) + " " +
            _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(_issuer);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_extend(_htlc_id, _issuer,
               _seconds_to_add, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm HTLC Extend?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_extend(_htlc_id, _issuer,
                     _seconds_to_add, false);
               p_GWallet->DoAssets(_issuer);
            }
         }
         response = result_obj;
         new ExtendHtlcResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

ExtendHtlcResponse::ExtendHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC Extend response"), true);
}
