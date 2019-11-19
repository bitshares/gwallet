#include "include/upgradeaccount.hpp"

#include <panels/commands.hpp>

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
   const auto _name = name->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   std::stringstream command;
   command << "upgrade_account " << _name << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(),
         _name, _("Confirm upgrade of account?"), _cli, _broadcast);

   if(!response.IsNull())
      new UpgradeAccountResponse(p_GWallet, response);
}

UpgradeAccountResponse::UpgradeAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Upgrade account response"), true);
}
