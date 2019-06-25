#include "../include/panels/upgradeaccount.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

UpgradeAccount::UpgradeAccount(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   name->Append(p_GWallet->strings.accounts);
   name->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UpgradeAccount::OnOk));
}

void UpgradeAccount::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account = name->GetValue().ToStdString();
   signed_transaction result_obj;
   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "upgrade_account " + account + " " + broadcast_value;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(account);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->upgrade_account(account, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]),
                  _("Confirm update of voting account?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->upgrade_account(account, true);
               p_GWallet->DoAssets(account);
            }
            response = result_obj;
            new UpgradeAccountResponse(p_GWallet, response);
         }
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

UpgradeAccountResponse::UpgradeAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Committee member response"), true);
}
