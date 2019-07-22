#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class VoteForCommittee: public wxScrolledWindow
{
public:
   VoteForCommittee(GWallet* gwallet);

protected:
   wxComboBox* voting_account;
   wxSearchCtrl* committee_member;
   wxChoice* approve;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchCommittee(wxCommandEvent &event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("VoteForCommitteeMember"), wxT("wxScrolledWindow"));
      voting_account = XRCCTRL(*this,"voting_account",wxComboBox);
      committee_member = XRCCTRL(*this,"committee_member",wxSearchCtrl);
      approve = XRCCTRL(*this,"approve",wxChoice);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class VoteForCommitteeMemberResponse: public wxScrolledWindow
{
public:
   VoteForCommitteeMemberResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("VoteForCommitteeMemberResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
