#include "../include/dialogs/settings.hpp"
#include "../include/gwallet.hpp"

Settings::Settings(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   wxString path_s;
   wxString server_s;
   int lang_s;
   bool all_set_s;
   p_GWallet->config->Read("WalletPath", &path_s);
   p_GWallet->config->Read("Server", &server_s);
   p_GWallet->config->Read("Lang", &lang_s);
   p_GWallet->config->Read("AllSet", &all_set_s);

   if(lang_s == wxLANGUAGE_CHINESE)
      lang->SetValue("Chinese");
   else if(lang_s == wxLANGUAGE_SPANISH)
      lang->SetValue("Spanish");
   else if(lang_s == wxLANGUAGE_ENGLISH)
      lang->SetValue("English");
   else
      lang->SetValue("English");

   if(all_set_s == true)
      all_set->SetValue(true);

   wallet_path->SetValue(path_s);
   server->SetValue(server_s);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Settings::OnSave));
   Connect(wxID_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Settings::OnClearEverything));
   ShowModal();
}

void Settings::OnClearEverything(wxCommandEvent &WXUNUSED(event))
{
   p_GWallet->config->DeleteAll();

   p_GWallet->state.is_noconfig = true;
   p_GWallet->DoState();

   Close();
}

void Settings::OnSave(wxCommandEvent& event)
{

}
