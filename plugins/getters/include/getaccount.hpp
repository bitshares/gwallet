#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class GetAccount: public wxScrolledWindow
{
public:
   GetAccount(GWallet* gwallet);

protected:
   wxSearchCtrl* account_name_or_id;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccount"), wxT("wxScrolledWindow"));
      account_name_or_id = XRCCTRL(*this,"account_name_or_id",wxSearchCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetAccountResponse: public wxPanel
{
public:
   GetAccountResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccountResponse"), wxT("wxPanel"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
