#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class TransferToBlind: public wxScrolledWindow
{
public:
   TransferToBlind(GWallet* gwallet);

protected:
   wxComboBox* from;
   wxComboBox* to;
   wxTextCtrl* amount;
   wxComboBox* asset;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferToBlind"), wxT("wxScrolledWindow"));
      from = XRCCTRL(*this,"from",wxComboBox);
      to = XRCCTRL(*this,"to",wxComboBox);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      asset = XRCCTRL(*this,"asset",wxComboBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class TransferToBlindResponse: public wxScrolledWindow
{
public:
   TransferToBlindResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferToBlindResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
