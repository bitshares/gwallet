#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class ImportBalanceDialog : public wxDialog {
protected:
   GWallet* p_GWallet;

   wxSearchCtrl* account;
   wxTextCtrl* key;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ImportBalanceDialog"), wxT("wxDialog"));
      account = XRCCTRL(*this,"account",wxSearchCtrl);
      key = XRCCTRL(*this,"key",wxTextCtrl);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

public:
   ImportBalanceDialog(GWallet* gwallet);
};
