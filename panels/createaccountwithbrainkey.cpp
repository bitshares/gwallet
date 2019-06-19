#include "../include/panels/createaccountwithbrainkey.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

CreateAccountWithBrainKey::CreateAccountWithBrainKey(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   registrar_account->Append(p_GWallet->strings.accounts);
   registrar_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateAccountWithBrainKey::OnOk));
   Connect(XRCID("referrer_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(CreateAccountWithBrainKey::OnSearchAccount), NULL, this);
   Connect(XRCID("get_brain_key"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateAccountWithBrainKey::OnGetBrainKey), NULL, this);
}

void CreateAccountWithBrainKey::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *referrer_account);
}

void CreateAccountWithBrainKey::OnGetBrainKey(wxCommandEvent& event)
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   int key = wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxCANCEL|wxICON_INFORMATION, this);
   if (key == wxOK)
      brain_key->SetValue(result.brain_priv_key);
}

void CreateAccountWithBrainKey::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto brain_key_value = brain_key->GetValue().ToStdString();
   const auto account_name_value = account_name->GetValue().ToStdString();
   const auto registrar_account_value = registrar_account->GetValue().ToStdString();
   const auto referrer_account_value = referrer_account->GetValue().ToStdString();

   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   wxWindow* wnd = p_GWallet->m_mgr.GetPane("Transfer").window;

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "create_account_with_brain_key \"" + brain_key_value + "\" " + account_name_value + " " + registrar_account_value + " " + referrer_account_value + " " + broadcast_value;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(registrar_account_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->create_account_with_brain_key(brain_key_value, account_name_value, registrar_account_value, referrer_account_value, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm create account?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION | wxMAXIMIZE_BOX, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->create_account_with_brain_key(brain_key_value, account_name_value, registrar_account_value, referrer_account_value, true);
               p_GWallet->DoAssets(registrar_account_value);
            }
         }
         response = result_obj;
         new CreateAccountWithBrainKeyResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(wnd, e.to_detail_string());
      }
   }
}

CreateAccountWithBrainKeyResponse::CreateAccountWithBrainKeyResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   signed_transaction result = any_response.As<signed_transaction>();

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

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

   gwallet->panels.p_commands->notebook->AddPage(this, "Create account with brain key response");
}
