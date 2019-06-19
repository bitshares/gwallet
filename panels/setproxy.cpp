#include "../include/panels/setproxy.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

SetProxy::SetProxy(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   DoVotingAccount();

   account_to_modify->Append(p_GWallet->strings.accounts);
   account_to_modify->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   voting_account->SetLabelText(current_voting_account);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetProxy::OnOk));
   Connect(XRCID("voting_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SetProxy::OnSearchAccount), NULL, this);
}

void SetProxy::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *voting_account);
}

void SetProxy::DoVotingAccount()
{
   const auto selected_account = p_GWallet->strings.selected_account.ToStdString();

   try
   {
      const auto account = p_GWallet->bitshares.wallet_api_ptr->get_account(selected_account);
      const auto account_id_string = std::string(object_id_type(account.options.voting_account));
      current_voting_account = p_GWallet->bitshares.wallet_api_ptr->get_account(account_id_string).name;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}

void SetProxy::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account_value = p_GWallet->strings.accounts[account_to_modify->GetCurrentSelection()].ToStdString();
   const auto voting_account_value = voting_account->GetValue().ToStdString();
   string broadcast_v = "false";
   if(broadcast->IsChecked())
      broadcast_v = "true";

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(account_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is invalid"));
      voting_account->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "set_voting_proxy " + account_value + " " + voting_account_value + " " + broadcast_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(account_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]),
               _("Confirm update of voting account?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, true);
               p_GWallet->DoAssets(account_value);
            }
            response = result_obj;
            new SetProxyResponse(p_GWallet, response);
         }
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

SetProxyResponse::SetProxyResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   signed_transaction result = any_response.As<signed_transaction>();

   const auto root = response_tree->AddRoot("Signed Transaction");

   const auto ref_block_num = response_tree->AppendItem(root, "Reference block number");
   response_tree->AppendItem(ref_block_num, to_string(result.ref_block_num));

   const auto ref_block_prefix = response_tree->AppendItem(root, "Reference block prefix");
   response_tree->AppendItem(ref_block_prefix, to_string(result.ref_block_prefix));

   const auto expiration = response_tree->AppendItem(root, "Expiration");
   response_tree->AppendItem(expiration, result.expiration.to_iso_string());

   const auto operations = response_tree->AppendItem(root, "Operations");
   response_tree->AppendItem(operations, fc::json::to_string(result.operations));

   const auto extensions = response_tree->AppendItem(root, "Extensions");
   response_tree->AppendItem(extensions, fc::json::to_string(result.extensions));

   const auto signatures = response_tree->AppendItem(root, "Signatures");
   response_tree->AppendItem(signatures, fc::json::to_string(result.signatures));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, "Set proxy response");
}
