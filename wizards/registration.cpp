#include "../include/wizards/registration.hpp"
#include "../include/panels/wallet.hpp"

Registration::Registration(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   path->SetValue(directory + wxT("/wallet.json"));

   Connect(XRCID("path_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Registration::OnPath));
   Connect(XRCID("endpoint_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Registration::OnEndpoint));
   Connect(XRCID("account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(Registration::OnSearchAccount), NULL, this);

   Connect(XRCID("Registration"), wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(Registration::PageChanging));
}

void Registration::OnPath(wxCommandEvent& WXUNUSED(event))
{
   const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   const wxString directory(f.GetPath());

   wxDirDialog dialog(this, _("Select your wallet directory"), directory, wxDD_NEW_DIR_BUTTON);
   if (dialog.ShowModal() == wxID_OK)
   {
      path->SetValue(dialog.GetPath() + "/wallet.json");
   }
}

void Registration::OnEndpoint(wxCommandEvent& WXUNUSED(event))
{
   const auto server = endpoint->GetValue();
   p_GWallet->config->Write("Server", server);
   p_GWallet->config->Flush();

   wxString path;
   if ( p_GWallet->config->Read("WalletPath", &path) ) {
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("t_connect"));
      try
      {
         p_GWallet->ProcessWindowEvent(event_connect);
         p_GWallet->bitshares.wallet_api_ptr->get_global_properties();

         wxMessageDialog dialog(NULL, _("Connected to server"), _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
         if (dialog.ShowModal() == wxID_OK)
            return;
      }
      catch(const fc::exception& e)
      {
         wxMessageDialog dialog(NULL, _("Some problem connecting"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
         if ( dialog.ShowModal() == wxID_OK )
            return;
      }
   }
}

void Registration::OnSearchAccount(wxCommandEvent& event)
{
   const auto keyword = event.GetString().ToStdString();

   wxArrayString choices;
   auto findings = p_GWallet->bitshares.database_api->lookup_accounts(keyword, 100);
   for(auto f : findings)
   {
      choices.Add(f.first);
   }

   wxSingleChoiceDialog dialog(this, _("Accounts found"), _("Please select an account"), choices);
   if (dialog.ShowModal() == wxID_OK)
      account->SetValue(dialog.GetStringSelection());
}

void Registration::PageChanging(wxWizardEvent& event)
{
   if(event.GetPage()->GetId() == XRCID("page2") and event.GetDirection())
   {
      if (wxFileExists(path->GetValue())) {
         wxMessageDialog dialog(NULL, _("Wallet file exists, please close the welcome wizard and use File->Open "
                                        "from menu to use it."), _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);
         if (dialog.ShowModal() == wxID_OK) {
            event.Veto();
            return;
         }
      }
      p_GWallet->config->Write("WalletPath", path->GetValue());
      p_GWallet->config->Flush();
   }

   else if(event.GetPage()->GetId() == XRCID("page3") and event.GetDirection())
   {
      const auto server = endpoint->GetValue();
      const auto pass = password->GetValue();
      const auto repeatpassword = repeat_password->GetValue();

      if (pass != repeatpassword) {
         wxMessageDialog dialog(NULL, _("Password and confirmation are not the same"),
               _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);

         if (dialog.ShowModal() == wxID_OK) {
            event.Veto();
            return;
         }
      } else if (pass == "" && repeatpassword == "") {
         wxMessageDialog dialog(NULL, _("Password or confirmation can not be empty"),
               _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);

         if (dialog.ShowModal() == wxID_OK) {
            event.Veto();
            return;
         }
      }
      p_GWallet->config->Write("Server", server);
      p_GWallet->config->Flush();

      if (!p_GWallet->state.is_connected) {
         wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("t_connect"));
         p_GWallet->ProcessWindowEvent(event_connect);
      }

      try {
         p_GWallet->bitshares.wallet_api_ptr->set_password(pass.ToStdString());
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
      try {
         p_GWallet->bitshares.wallet_api_ptr->unlock(pass.ToStdString());
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
      p_GWallet->state.is_locked = false;
      p_GWallet->state.is_new = false;
      p_GWallet->state.is_unlocked = true;
      p_GWallet->DoState();
   }

   else if(event.GetPage()->GetId() == XRCID("page5") and event.GetDirection())
   {
      wxWindowDisabler disableAll;
      wxBusyInfo wait(_("Please wait, setting up everything ..."));
      wxTheApp->Yield();

      auto acc = account->GetValue();
      auto key = private_key->GetValue();

      if (acc == "") {
         wxMessageDialog dialog(NULL, _("Account can't be empty"), _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);

         if (dialog.ShowModal() == wxID_OK) {
            event.Veto();
         }
      } else {
         try {
            p_GWallet->bitshares.wallet_api_ptr->get_account(acc.ToStdString());
         }
         catch (const fc::exception &e) {
            p_GWallet->OnError(this, "Account is invalid");
            account->SetFocus();
            return;
         }
      }

      if (key == "") {
         wxMessageDialog dialog(NULL, _("Private key can't be empty"), _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);

         if (dialog.ShowModal() == wxID_OK) {
            event.Veto();
         }
      } else {
         // todo: do some validation on private key length and prefix?
      }

      try {
         p_GWallet->bitshares.wallet_api_ptr->import_key(acc.ToStdString(), key.ToStdString());
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         event.Veto();
      }

      p_GWallet->DoAccounts();
      p_GWallet->DoAssets(acc.ToStdString());

      p_GWallet->config->Write("AllSet", true);
      p_GWallet->config->Flush();

      p_GWallet->state.is_account_linked = true;
      p_GWallet->strings.selected_account = acc;

      if (!p_GWallet->state.modes_created) {
         p_GWallet->DoModes();
         p_GWallet->state.modes_created = true;
      }

      p_GWallet->DoState();

      p_GWallet->panels.p_wallet->EnableOperations();
   }
}

