#include "../include/panels/getaccount.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

GetAccount::GetAccount(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetAccount::OnOk));
   Connect(XRCID("account_name_or_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetAccount::OnSearchAccount), NULL, this);
}

void GetAccount::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *account_name_or_id);
}

void GetAccount::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account = account_name_or_id->GetValue().ToStdString();
   account_object result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   try
   {
      result_obj = p_GWallet->bitshares.wallet_api_ptr->get_account(account);
      response = result_obj;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is not a committee member"));
      account_name_or_id->SetFocus();
      return;
   }

   new GetAccountResponse(p_GWallet, response);

   if(cli->IsChecked())
   {
      auto command = "get_account " + account;
      p_GWallet->panels.p_cli->DoCommand(command);
   }
}

GetAccountResponse::GetAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   account_object result = any_response.As<account_object>();

   // Todo: Too much code just to have the benefit of keys of objects translated
   // Probably better to loop throw the object and build the tree

   const auto root = response_tree->AddRoot(_("Account object"));

   const auto id = response_tree->AppendItem(root, "ID");
   response_tree->AppendItem(id, string(object_id_type(result.id)));

   const auto membership_expiration_date = response_tree->AppendItem(root, _("Membership expiration date"));
   response_tree->AppendItem(membership_expiration_date, result.membership_expiration_date.to_iso_string());

   const auto registrar = response_tree->AppendItem(root, _("Registrar"));
   response_tree->AppendItem(registrar, string(object_id_type(result.registrar)));

   const auto referrer = response_tree->AppendItem(root, _("Referrer"));
   response_tree->AppendItem(referrer, string(object_id_type(result.referrer)));

   const auto lifetime_referrer = response_tree->AppendItem(root, _("Lifetime referrer"));
   response_tree->AppendItem(lifetime_referrer, string(object_id_type(result.lifetime_referrer)));

   const auto network_fee_percentage = response_tree->AppendItem(root, _("Network fee percentage"));
   response_tree->AppendItem(network_fee_percentage, to_string(result.network_fee_percentage));

   const auto lifetime_referrer_fee_percentage = response_tree->AppendItem(root, _("Lifetime referrer fee percentage"));
   response_tree->AppendItem(lifetime_referrer_fee_percentage, to_string(result.lifetime_referrer_fee_percentage));

   const auto referrer_rewards_percentage = response_tree->AppendItem(root, _("Referrer rewards percentage"));
   response_tree->AppendItem(referrer_rewards_percentage, to_string(result.referrer_rewards_percentage));

   const auto name = response_tree->AppendItem(root, _("Name"));
   response_tree->AppendItem(name, result.name);

   const auto owner = response_tree->AppendItem(root, _("Owner"));
   const auto owner_weight_threshold = response_tree->AppendItem(owner, _("Weight threshold"));
   response_tree->AppendItem(owner_weight_threshold, to_string(result.owner.weight_threshold));
   const auto owner_account_auths = response_tree->AppendItem(owner, _("Account authorities"));
   response_tree->AppendItem(owner_account_auths, fc::json::to_string(result.owner.account_auths));
   const auto owner_key_auths = response_tree->AppendItem(owner, _("Key authorities"));
   response_tree->AppendItem(owner_key_auths, fc::json::to_string(result.owner.key_auths));
   const auto owner_address_auths = response_tree->AppendItem(owner, _("Address authorities"));
   response_tree->AppendItem(owner_address_auths, fc::json::to_string(result.owner.address_auths));

   const auto active = response_tree->AppendItem(root, _("Active"));
   const auto active_weight_threshold = response_tree->AppendItem(active, _("Weight threshold"));
   response_tree->AppendItem(active_weight_threshold, to_string(result.active.weight_threshold));
   const auto active_account_auths = response_tree->AppendItem(active, _("Account authorities"));
   response_tree->AppendItem(active_account_auths, fc::json::to_string(result.active.account_auths));
   const auto active_key_auths = response_tree->AppendItem(active, _("Key authorities"));
   response_tree->AppendItem(active_key_auths, fc::json::to_string(result.active.key_auths));
   const auto active_address_auths = response_tree->AppendItem(active, _("Address authorities"));
   response_tree->AppendItem(active_address_auths, fc::json::to_string(result.active.address_auths));

   const auto options = response_tree->AppendItem(root, _("Options"));
   const auto memo_key = response_tree->AppendItem(options, _("Memo key"));
   response_tree->AppendItem(memo_key, string(result.options.memo_key));
   const auto voting_account = response_tree->AppendItem(options, _("Voting account"));
   response_tree->AppendItem(voting_account, string(object_id_type(result.options.voting_account)));
   const auto num_witness = response_tree->AppendItem(options, _("Number of witness"));
   response_tree->AppendItem(num_witness, to_string(result.options.num_witness));
   const auto num_committee = response_tree->AppendItem(options, _("Number of committee"));
   response_tree->AppendItem(num_committee, to_string(result.options.num_committee));
   const auto votes = response_tree->AppendItem(options, _("Votes"));
   response_tree->AppendItem(votes, fc::json::to_string(result.options.votes));
   const auto extensions = response_tree->AppendItem(options, _("Extensions"));
   response_tree->AppendItem(extensions, fc::json::to_string(result.options.extensions));

   const auto statistics = response_tree->AppendItem(root, _("Statistics"));
   response_tree->AppendItem(statistics, fc::json::to_string(result.statistics));

   const auto whitelisting_accounts = response_tree->AppendItem(root, _("Whitelisting accounts"));
   response_tree->AppendItem(whitelisting_accounts, fc::json::to_string(result.whitelisting_accounts));

   const auto blacklisting_accounts = response_tree->AppendItem(root, _("Blacklisting accounts"));
   response_tree->AppendItem(blacklisting_accounts, fc::json::to_string(result.blacklisting_accounts));

   const auto whitelisted_accounts = response_tree->AppendItem(root, _("Whitelisted accounts"));
   response_tree->AppendItem(whitelisted_accounts, fc::json::to_string(result.whitelisted_accounts));

   const auto blacklisted_accounts = response_tree->AppendItem(root, _("Blacklisted accounts"));
   response_tree->AppendItem(blacklisted_accounts, fc::json::to_string(result.blacklisted_accounts));

   const auto owner_special_authority = response_tree->AppendItem(root, _("Owner special authority"));
   response_tree->AppendItem(owner_special_authority, fc::json::to_string(result.owner_special_authority));

   const auto active_special_authority = response_tree->AppendItem(root, _("Active special authority"));
   response_tree->AppendItem(active_special_authority, fc::json::to_string(result.active_special_authority));

   const auto top_n_control_flags = response_tree->AppendItem(root, _("Top n control flags"));
   response_tree->AppendItem(top_n_control_flags, to_string(result.top_n_control_flags));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, _("Get account response"), true);
}
