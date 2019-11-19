#include "include/voteforwitness.hpp"

#include <panels/commands.hpp>

VoteForWitness::VoteForWitness(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   voting_account->Append(p_GWallet->strings.accounts);
   voting_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(VoteForWitness::OnOk));
   Connect(XRCID("witness"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(VoteForWitness::OnSearchWitness), NULL, this);

}

void VoteForWitness::OnSearchWitness(wxCommandEvent& event)
{
   p_GWallet->DoSearchWitness(event.GetString(), *witness);
}

void VoteForWitness::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _voting_account = voting_account->GetValue().ToStdString();
   const auto _witness = witness->GetValue().ToStdString();
   auto _approve = true;
   if(approve->GetSelection() == 1) _approve = false;
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   std::stringstream command;
   command << "vote_for_witness " << _voting_account << " " << _witness << " " << std::boolalpha << _approve
           << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _voting_account,
         _("Confirm vote for witness?"), _cli, _broadcast);

   if(!response.IsNull())
      new VoteForWitnessResponse(p_GWallet, response);
}

VoteForWitnessResponse::VoteForWitnessResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Vote for witness response"), true);
}
