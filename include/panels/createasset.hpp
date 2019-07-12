#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/richmsgdlg.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class CreateAsset : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxComboBox* issuer;
   wxTextCtrl* symbol;
   wxSpinCtrl* precision;
   wxTextCtrl* common;
   wxTextCtrl* bitasset_opts;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateAsset"), wxT("wxScrolledWindow"));
      issuer = XRCCTRL(*this,"issuer",wxComboBox);
      symbol = XRCCTRL(*this,"symbol",wxTextCtrl);
      precision = XRCCTRL(*this,"precision",wxSpinCtrl);
      common = XRCCTRL(*this,"common",wxTextCtrl);
      bitasset_opts = XRCCTRL(*this,"bitasset_opts",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnGenerateCommon(wxCommandEvent& event);


public:
   CreateAsset(GWallet* gwallet);
};

class CreateAssetResponse: public wxScrolledWindow
{
public:
   CreateAssetResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateAssetResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
