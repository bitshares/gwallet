#include "../include/dialogs/importbalance.hpp"
#include "../include/gwallet.hpp"

ImportBalanceDialog::ImportBalanceDialog(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImportBalanceDialog::OnOk));
   Connect(XRCID("account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(ImportBalanceDialog::OnSearchAccount), NULL, this);
   ShowModal();
}

void ImportBalanceDialog::OnOk(wxCommandEvent &WXUNUSED(event)) {

   if(!account->IsEmpty() && !key->IsEmpty()) {
      const auto acct = account->GetValue();
      const auto pkey = key->GetValue();

      try {
         p_GWallet->bitshares.wallet_api_ptr->import_balance(acct.ToStdString(), {pkey.ToStdString()}, true);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         return;
      }
      p_GWallet->DoAssets(acct.ToStdString());

      Close(true);

      wxMessageDialog dialog(NULL, _("Account balance imported"), _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   } else {
      wxMessageDialog dialog(NULL, _("Account and key can not be empty"), _("Error"), wxNO_DEFAULT | wxOK | wxICON_ERROR);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
}

void ImportBalanceDialog::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *account);
}

