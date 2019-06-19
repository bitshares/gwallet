#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/wizard.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/busyinfo.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Registration : public wxWizard {
protected:
   wxWizardPageSimple* page2;
   wxTextCtrl* path;
   wxBitmapButton* path_button;
   wxWizardPageSimple* page3;
   wxTextCtrl* endpoint;
   wxBitmapButton* endpoint_button;
   wxTextCtrl* password;
   wxTextCtrl* repeat_password;
   wxWizardPageSimple* page4;
   wxRadioButton* import_account_radio;
   wxRadioButton* create_account_radio;
   wxWizardPageSimple* page5;
   wxSearchCtrl* account;
   wxTextCtrl* private_key;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Registration"), wxT("wxWizard"));
      page1 = XRCCTRL(*this,"page1",wxWizardPageSimple);
      page2 = XRCCTRL(*this,"page2",wxWizardPageSimple);
      path = XRCCTRL(*this,"path",wxTextCtrl);
      path_button = XRCCTRL(*this,"path_button",wxBitmapButton);
      page3 = XRCCTRL(*this,"page3",wxWizardPageSimple);
      endpoint = XRCCTRL(*this,"endpoint",wxTextCtrl);
      endpoint_button = XRCCTRL(*this,"endpoint_button",wxBitmapButton);
      password = XRCCTRL(*this,"password",wxTextCtrl);
      repeat_password = XRCCTRL(*this,"repeat_password",wxTextCtrl);
      page4 = XRCCTRL(*this,"page4",wxWizardPageSimple);
      import_account_radio = XRCCTRL(*this,"import_account_radio",wxRadioButton);
      create_account_radio = XRCCTRL(*this,"create_account_radio",wxRadioButton);
      page5 = XRCCTRL(*this,"page5",wxWizardPageSimple);
      account = XRCCTRL(*this,"account",wxSearchCtrl);
      private_key = XRCCTRL(*this,"private_key",wxTextCtrl);
   }
   void OnPath(wxCommandEvent& event);
   void OnEndpoint(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);
   void PageChanging(wxWizardEvent& event);

public:
   GWallet* p_GWallet;

   wxWizardPageSimple* page1;

   Registration(GWallet* gwallet);
};
