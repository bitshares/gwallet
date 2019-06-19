#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class SetProxy: public wxScrolledWindow
{
public:
   SetProxy(GWallet* gwallet);

protected:
   wxComboBox* account_to_modify;
   wxSearchCtrl* voting_account;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   wxString current_voting_account;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);
   void DoVotingAccount();

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SetProxy"), wxT("wxScrolledWindow"));
      account_to_modify = XRCCTRL(*this,"account_to_modify",wxComboBox);
      voting_account = XRCCTRL(*this,"voting_account",wxSearchCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class SetProxyResponse: public wxScrolledWindow
{
public:
   SetProxyResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
         wxXmlResource::Get()->LoadObject(this,parent,wxT("SetProxyResponse"), wxT("wxScrolledWindow"));
         response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
