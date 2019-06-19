#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class RegisterAccount : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxTextCtrl* name;
   wxTextCtrl* owner;
   wxTextCtrl* active;
   wxComboBox* registrar_account;
   wxSearchCtrl* referrer_account;
   wxSpinCtrl* referrer_percent;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("RegisterAccount"), wxT("wxScrolledWindow"));
      name = XRCCTRL(*this,"name",wxTextCtrl);
      owner = XRCCTRL(*this,"owner",wxTextCtrl);
      active = XRCCTRL(*this,"active",wxTextCtrl);
      registrar_account = XRCCTRL(*this,"registrar_account",wxComboBox);
      referrer_account = XRCCTRL(*this,"referrer_account",wxSearchCtrl);
      referrer_percent = XRCCTRL(*this,"referrer_percent",wxSpinCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);

public:
   RegisterAccount(GWallet* gwallet);
};

class RegisterAccountResponse: public wxScrolledWindow
{
public:
   RegisterAccountResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("RegisterAccountResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
