#include "../include/panels/getwitness.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

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
   witness_object result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   try
   {
      result_obj = p_GWallet->bitshares.wallet_api_ptr->get_witness(_owner_account);
      response = result_obj;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is not a witness"));
      owner_account->SetFocus();
      return;
   }

   new GetWitnessResponse(p_GWallet, response);

   if(cli->IsChecked())
   {
      auto command = "get_witness " + _owner_account;
      p_GWallet->panels.p_cli->DoCommand(command);
   }
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

   const auto witness_account = response_tree->AppendItem(root, _("Witness account"));
   response_tree->AppendItem(witness_account, string(object_id_type(result.witness_account)));

   const auto last_aslot = response_tree->AppendItem(root, _("Last slot"));
   response_tree->AppendItem(last_aslot, wxNumberFormatter::ToString((long)result.last_aslot));

   const auto signing_key = response_tree->AppendItem(root, _("Signing key"));
   response_tree->AppendItem(signing_key, fc::json::to_string(result.signing_key));

   const auto pay_vb = response_tree->AppendItem(root, _("Payment vesting balance"));
   response_tree->AppendItem(pay_vb, fc::json::to_string(result.pay_vb));

   const auto vote_id = response_tree->AppendItem(root, "Vote ID");
   response_tree->AppendItem(vote_id, fc::json::to_string(result.vote_id));

   const auto total_votes = response_tree->AppendItem(root, "Total votes");
   response_tree->AppendItem(total_votes, wxNumberFormatter::ToString((long)(result.total_votes/pow(10, 5))));

   const auto url = response_tree->AppendItem(root, "Url");
   response_tree->AppendItem(url, fc::json::to_string(result.url));

   const auto total_missed = response_tree->AppendItem(root, _("Total missed"));
   response_tree->AppendItem(total_missed, wxNumberFormatter::ToString(result.total_missed));

   const auto last_confirmed_block_num = response_tree->AppendItem(root, _("Last confirmed block number"));
   response_tree->AppendItem(last_confirmed_block_num, wxNumberFormatter::ToString((long)result.last_confirmed_block_num));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, _("Witness response"), true);
}
