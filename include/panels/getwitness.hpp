#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetWitness: public wxScrolledWindow
{
public:
   GetWitness(GWallet* gwallet);

protected:
   wxSearchCtrl* owner_account;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetWitness"), wxT("wxScrolledWindow"));
      owner_account = XRCCTRL(*this,"owner_account",wxSearchCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetWitnessResponse: public wxScrolledWindow
{
public:
   GetWitnessResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetWitnessResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};