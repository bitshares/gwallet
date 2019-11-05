#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class ReceiveBlindTransfer: public wxScrolledWindow
{
public:
   ReceiveBlindTransfer(GWallet* gwallet);

protected:
   wxTextCtrl* confirmation_receipt;
   wxTextCtrl* opt_from;
   wxTextCtrl* opt_memo;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ReceiveBlindTransfer"), wxT("wxScrolledWindow"));
      confirmation_receipt = XRCCTRL(*this,"confirmation_receipt",wxTextCtrl);
      opt_from = XRCCTRL(*this,"opt_from",wxTextCtrl);
      opt_memo = XRCCTRL(*this,"opt_memo",wxTextCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class ReceiveBlindTransferResponse: public wxScrolledWindow
{
public:
   ReceiveBlindTransferResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ReceiveBlindTransferResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
