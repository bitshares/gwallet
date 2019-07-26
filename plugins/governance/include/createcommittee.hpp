#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class CreateCommittee: public wxScrolledWindow
{
public:
   CreateCommittee(GWallet* gwallet);

protected:
   wxComboBox* owner_account;
   wxTextCtrl* url;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateCommitteeMember"), wxT("wxScrolledWindow"));
      owner_account = XRCCTRL(*this,"owner_account",wxComboBox);
      url = XRCCTRL(*this,"url",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class CreateCommitteeResponse: public wxScrolledWindow
{
public:
   CreateCommitteeResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateCommitteeMemberResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
