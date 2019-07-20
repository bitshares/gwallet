#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class ExtendHtlc : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxComboBox* issuer;
   wxSearchCtrl* htlc_id;
   wxSpinCtrl* minutes_to_add;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ExtendHtlc"), wxT("wxScrolledWindow"));
      issuer = XRCCTRL(*this,"issuer",wxComboBox);
      htlc_id = XRCCTRL(*this,"htlc_id",wxSearchCtrl);
      minutes_to_add = XRCCTRL(*this,"minutes_to_add",wxSpinCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchID(wxCommandEvent &event);

public:
   ExtendHtlc(GWallet* gwallet);
};

class ExtendHtlcResponse: public wxScrolledWindow
{
public:
   ExtendHtlcResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ExtendHtlcResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
