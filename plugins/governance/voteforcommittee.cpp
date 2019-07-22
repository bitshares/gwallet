#include "include/voteforcommittee.hpp"

#include <panels/commands.hpp>

VoteForCommittee::VoteForCommittee(GWallet* gwallet)
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/governance/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   voting_account->Append(p_GWallet->strings.accounts);
   voting_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(VoteForCommittee::OnOk));
   Connect(XRCID("committee_member"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(VoteForCommittee::OnSearchCommittee), NULL, this);

}

void VoteForCommittee::OnSearchCommittee(wxCommandEvent& event)
{
   p_GWallet->DoSearchCommittee(event.GetString(), *committee_member);
}

void VoteForCommittee::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _voting_account = voting_account->GetValue().ToStdString();
   const auto _committee_member = committee_member->GetValue().ToStdString();
   auto _approve = true;
   if(approve->GetSelection() == 1) _approve = false;
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "vote_for_committee_member " << _voting_account << " " << _committee_member << " " << std::boolalpha << _approve
           << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _voting_account,
         _("Confirm vote for committee member?"), _cli, _broadcast);

   if(!response.IsNull())
      new VoteForCommitteeMemberResponse(p_GWallet, response);
}

VoteForCommitteeMemberResponse::VoteForCommitteeMemberResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Vote for committee member response"), true);
}
