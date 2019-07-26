#include "include/createcommittee.hpp"

#include <panels/commands.hpp>

CreateCommittee::CreateCommittee(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   owner_account->Append(p_GWallet->strings.accounts);
   owner_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateCommittee::OnOk));
}

void CreateCommittee::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _owner_account = owner_account->GetValue().ToStdString();
   const auto _url = url->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "create_committee_member " << _owner_account << " \"" << _url << "\" " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _owner_account,
         _("Confirm create committee member?"), _cli, _broadcast);

   if(!response.IsNull())
      new CreateCommitteeResponse(p_GWallet, response);
}

CreateCommitteeResponse::CreateCommitteeResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Create committee member response"), true);
}
