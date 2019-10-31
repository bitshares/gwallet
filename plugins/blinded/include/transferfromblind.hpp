#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class TransferFromBlind: public wxScrolledWindow
{
public:
   TransferFromBlind(GWallet* gwallet);

protected:
   wxComboBox* from_blind_account_key_or_label;
   wxSearchCtrl* to_account_id_or_name;
   wxTextCtrl* amount;
   wxComboBox* asset_symbol;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferFromBlind"), wxT("wxScrolledWindow"));
      from_blind_account_key_or_label = XRCCTRL(*this,"from_blind_account_key_or_label",wxComboBox);
      to_account_id_or_name = XRCCTRL(*this,"to_account_id_or_name",wxSearchCtrl);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      asset_symbol = XRCCTRL(*this,"asset_symbol",wxComboBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class TransferFromBlindResponse: public wxScrolledWindow
{
public:
   TransferFromBlindResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferFromBlindResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
