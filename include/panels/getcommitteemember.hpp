#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetCommitteeMember: public wxScrolledWindow
{
public:
   GetCommitteeMember(GWallet* gwallet);

protected:
   wxSearchCtrl* owner_account;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetCommitteeMember"), wxT("wxScrolledWindow"));
      owner_account = XRCCTRL(*this,"owner_account",wxSearchCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetCommitteeMemberResponse: public wxScrolledWindow
{
public:
   GetCommitteeMemberResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetCommitteeMemberResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};