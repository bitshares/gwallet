#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class RedeemHtlc : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxComboBox* issuer;
   wxSearchCtrl* htlc_id;
   wxTextCtrl* preimage;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("RedeemHtlc"), wxT("wxScrolledWindow"));
      issuer = XRCCTRL(*this,"issuer",wxComboBox);
      htlc_id = XRCCTRL(*this,"htlc_id",wxSearchCtrl);
      preimage = XRCCTRL(*this,"preimage",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchID(wxCommandEvent &event);

public:
   RedeemHtlc(GWallet* gwallet);
};

class RedeemHtlcResponse: public wxScrolledWindow
{
public:
   RedeemHtlcResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("RedeemHtlcResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
