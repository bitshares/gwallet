#include "include/getcommitteemember.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetCommitteeMember::GetCommitteeMember(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetCommitteeMember::OnOk));
   Connect(XRCID("owner_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetCommitteeMember::OnSearchAccount),
         NULL, this);
}

void GetCommitteeMember::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *owner_account);
}

void GetCommitteeMember::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _owner_account = owner_account->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   if(!p_GWallet->panels.p_commands->ValidateAccount(owner_account))
      return;

   stringstream command;
   command << "get_committee_member " << _owner_account;

   auto response = p_GWallet->panels.p_commands->ExecuteGetterCommand<committee_member_object>(command.str(), _cli,
         _("Account is not a committee member"));

   if(!response.IsNull())
      new GetCommitteeMemberResponse(p_GWallet, response);
}

GetCommitteeMemberResponse::GetCommitteeMemberResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   committee_member_object result = any_response.As<committee_member_object>();

   const auto root = response_tree->AddRoot(_("Committee member object"));

   const auto id = response_tree->AppendItem(root, "ID");
   response_tree->AppendItem(id, string(object_id_type(result.id)));

   const auto committee_member_account = response_tree->AppendItem(root, _("Committee member account"));
   response_tree->AppendItem(committee_member_account, string(object_id_type(result.committee_member_account)));

   const auto vote_id = response_tree->AppendItem(root, _("Vote ID"));
   response_tree->AppendItem(vote_id, fc::json::to_string(result.vote_id));

   const auto total_votes = response_tree->AppendItem(root, _("Total votes"));
   response_tree->AppendItem(total_votes, wxNumberFormatter::ToString((long)(result.total_votes/pow(10, 5))));

   const auto url = response_tree->AppendItem(root, "Url");
   response_tree->AppendItem(url, fc::json::to_string(result.url));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, _("Committee member response"), true);
}
