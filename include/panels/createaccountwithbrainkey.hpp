#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class CreateAccountWithBrainKey : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxTextCtrl* brain_key;
   wxButton* get_brain_key;
   wxTextCtrl* account_name;
   wxComboBox* registrar_account;
   wxSearchCtrl* referrer_account;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateAccountWithBrainKey"), wxT("wxScrolledWindow"));
      brain_key = XRCCTRL(*this,"brain_key",wxTextCtrl);
      get_brain_key = XRCCTRL(*this,"get_brain_key",wxButton);
      account_name = XRCCTRL(*this,"account_name",wxTextCtrl);
      registrar_account = XRCCTRL(*this,"registrar_account",wxComboBox);
      referrer_account = XRCCTRL(*this,"referrer_account",wxSearchCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);
   void OnGetBrainKey(wxCommandEvent &event);

public:
   CreateAccountWithBrainKey(GWallet* gwallet);
};

class CreateAccountWithBrainKeyResponse: public wxScrolledWindow
{
public:
   CreateAccountWithBrainKeyResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateAccountWithBrainKeyResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
