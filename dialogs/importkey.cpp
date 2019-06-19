#include "../include/dialogs/importkey.hpp"
#include "../include/gwallet.hpp"

ImportKeyDialog::ImportKeyDialog(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImportKeyDialog::OnOk));
   Connect(XRCID("account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(ImportKeyDialog::OnSearchAccount), NULL, this);
   ShowModal();
}

void ImportKeyDialog::OnOk(wxCommandEvent &WXUNUSED(event)) {

   if(!account->IsEmpty() && !key->IsEmpty()) {
      const auto acct = account->GetValue();
      const auto pkey = key->GetValue();

      try {
         p_GWallet->bitshares.wallet_api_ptr->import_key(acct.ToStdString(), pkey.ToStdString());
      }
      catch (const fc::exception &e) {
         //p_GWallet->OnError(_("Account/Key pair is invalid, please try again."));
         p_GWallet->OnError(this, e.to_detail_string());
         return;
      }

      p_GWallet->config->Write("AllSet", true);
      p_GWallet->state.is_account_linked = true;
      p_GWallet->config->Flush();

      p_GWallet->DoAccounts();
      p_GWallet->DoAssets(acct.ToStdString());

      p_GWallet->bitshares.wallet_api_ptr->save_wallet_file();

      Close(true);

      wxMessageDialog dialog(NULL, _("Account imported"), _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   } else {
      wxMessageDialog dialog(NULL, _("Account and key can not be empty"), _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
}

void ImportKeyDialog::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *account);
}
