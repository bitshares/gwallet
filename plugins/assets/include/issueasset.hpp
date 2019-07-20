#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class IssueAsset : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxSearchCtrl* to_account;
   wxTextCtrl* amount;
   wxSearchCtrl* symbol;
   wxTextCtrl* memo;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("IssueAsset"), wxT("wxScrolledWindow"));
      to_account = XRCCTRL(*this,"to_account",wxSearchCtrl);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      symbol = XRCCTRL(*this,"symbol",wxSearchCtrl);
      memo = XRCCTRL(*this,"memo",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);
   void OnSearchAsset(wxCommandEvent &event);

public:
   IssueAsset(GWallet* gwallet);
};

class IssueAssetResponse: public wxScrolledWindow
{
public:
   IssueAssetResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("IssueAssetResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
