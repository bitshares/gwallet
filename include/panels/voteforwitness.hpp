#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class VoteForWitness: public wxScrolledWindow
{
public:
   VoteForWitness(GWallet* gwallet);

protected:
   wxComboBox* voting_account;
   wxSearchCtrl* witness;
   wxChoice* approve;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchWitness(wxCommandEvent &event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("VoteForWitness"), wxT("wxScrolledWindow"));
      voting_account = XRCCTRL(*this,"voting_account",wxComboBox);
      witness = XRCCTRL(*this,"witness",wxSearchCtrl);
      approve = XRCCTRL(*this,"approve",wxChoice);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class VoteForWitnessResponse: public wxScrolledWindow
{
public:
   VoteForWitnessResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("VoteForWitnessResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
