#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class Settings : public wxDialog {
protected:
   GWallet* p_GWallet;

   wxTextCtrl* wallet_path;
   wxTextCtrl* server;
   wxTextCtrl* lang;
   wxCheckBox* all_set;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Settings"), wxT("wxDialog"));
      wallet_path = XRCCTRL(*this,"wallet_path",wxTextCtrl);
      server = XRCCTRL(*this,"server",wxTextCtrl);
      lang = XRCCTRL(*this,"lang",wxTextCtrl);
      all_set = XRCCTRL(*this,"all_set",wxCheckBox);
   }
   void OnClearEverything(wxCommandEvent& event);
   void OnSave(wxCommandEvent& event);
   void OnCancel(wxCommandEvent& event);

public:
   Settings(GWallet* gwallet);
};
