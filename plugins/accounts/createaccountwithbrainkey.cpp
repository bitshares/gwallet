#include "include/createaccountwithbrainkey.hpp"

#include <panels/commands.hpp>

CreateAccountWithBrainKey::CreateAccountWithBrainKey(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   registrar_account->Append(p_GWallet->strings.accounts);
   registrar_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   brain_key->SetValidator(*p_GWallet->panels.p_commands->empty_validator);
   account_name->SetValidator(*p_GWallet->panels.p_commands->empty_validator);
   referrer_account->SetValue(p_GWallet->strings.selected_account);

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
   if(!Validate())
      return;

   const auto _brain_key = brain_key->GetValue().ToStdString();
   const auto _account_name = account_name->GetValue().ToStdString();
   const auto _registrar_account = registrar_account->GetValue().ToStdString();
   const auto _referrer_account = referrer_account->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "create_account_with_brain_key \"" << _brain_key << "\" " << _account_name << " " << _registrar_account
           << " " << _referrer_account << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _registrar_account,
         _("Confirm create account?"), _cli, _broadcast);

   if(!response.IsNull())
      new CreateAccountWithBrainKeyResponse(p_GWallet, response);
}

CreateAccountWithBrainKeyResponse::CreateAccountWithBrainKeyResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Create account with brain key response"), true);
}
