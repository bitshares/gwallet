#include "include/getwitness.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetWitness::GetWitness(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetWitness::OnOk));
   Connect(XRCID("owner_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetWitness::OnSearchAccount), NULL, this);
}

void GetWitness::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *owner_account);
}

void GetWitness::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _owner_account = owner_account->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   if(!p_GWallet->panels.p_commands->ValidateAccount(owner_account))
      return;

   std::stringstream command;
   command << "get_witness " << _owner_account;

   auto response = p_GWallet->panels.p_commands->ExecuteGetterCommand<witness_object>(command.str(), _cli,
         _("Account is not a witness"));

   if(!response.IsNull())
      new GetWitnessResponse(p_GWallet, response);
}

GetWitnessResponse::GetWitnessResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   witness_object result = any_response.As<witness_object>();

   const auto root = response_tree->AddRoot(_("Witness object"));

   const auto id = response_tree->AppendItem(root, "ID");
   response_tree->AppendItem(id, string(object_id_type(result.id)));
   response_tree->Expand(id);

   const auto witness_account = response_tree->AppendItem(root, _("Witness account"));
   response_tree->AppendItem(witness_account, string(object_id_type(result.witness_account)));
   response_tree->Collapse(witness_account);

   const auto last_aslot = response_tree->AppendItem(root, _("Last slot"));
   response_tree->AppendItem(last_aslot, wxNumberFormatter::ToString((long)result.last_aslot));
   response_tree->Collapse(last_aslot);

   const auto signing_key = response_tree->AppendItem(root, _("Signing key"));
   response_tree->AppendItem(signing_key, fc::json::to_string(result.signing_key));
   response_tree->Collapse(signing_key);

   const auto pay_vb = response_tree->AppendItem(root, _("Payment vesting balance"));
   response_tree->AppendItem(pay_vb, fc::json::to_string(result.pay_vb));
   response_tree->Collapse(pay_vb);

   const auto vote_id = response_tree->AppendItem(root, "Vote ID");
   response_tree->AppendItem(vote_id, fc::json::to_string(result.vote_id));
   response_tree->Collapse(vote_id);

   const auto total_votes = response_tree->AppendItem(root, "Total votes");
   response_tree->AppendItem(total_votes, wxNumberFormatter::ToString((long)(result.total_votes/pow(10, 5))));
   response_tree->Collapse(total_votes);

   const auto url = response_tree->AppendItem(root, "Url");
   response_tree->AppendItem(url, fc::json::to_string(result.url));
   response_tree->Collapse(url);

   const auto total_missed = response_tree->AppendItem(root, _("Total missed"));
   response_tree->AppendItem(total_missed, wxNumberFormatter::ToString(result.total_missed));
   response_tree->Collapse(total_missed);

   const auto last_confirmed_block_num = response_tree->AppendItem(root, _("Last confirmed block number"));
   response_tree->AppendItem(last_confirmed_block_num, wxNumberFormatter::ToString((long)result.last_confirmed_block_num));
   response_tree->Collapse(last_confirmed_block_num);

   response_tree->Expand(root);

   gwallet->panels.p_commands->notebook->AddPage(this, _("Witness response"), true);
}
