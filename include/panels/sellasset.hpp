#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class SellAsset: public wxScrolledWindow
{
public:
   SellAsset(GWallet* gwallet);

protected:
   GWallet* p_GWallet;

   wxComboBox* seller;
   wxTextCtrl* sell_amount;
   wxComboBox* sell_asset;
   wxTextCtrl* receive_amount;
   wxSearchCtrl* receive_asset;
   wxDatePickerCtrl* date;
   wxTimePickerCtrl* time;
   wxCheckBox* fill_or_kill;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:

   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SellAsset"), wxT("wxScrolledWindow"));
      seller = XRCCTRL(*this,"seller",wxComboBox);
      sell_amount = XRCCTRL(*this,"sell_amount",wxTextCtrl);
      sell_asset = XRCCTRL(*this,"sell_asset",wxComboBox);
      receive_amount = XRCCTRL(*this,"receive_amount",wxTextCtrl);
      receive_asset = XRCCTRL(*this,"receive_asset",wxSearchCtrl);
      date = XRCCTRL(*this,"date",wxDatePickerCtrl);
      time = XRCCTRL(*this,"time",wxTimePickerCtrl);
      fill_or_kill = XRCCTRL(*this,"fill_or_kill",wxCheckBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class SellAssetResponse: public wxScrolledWindow
{
public:
   SellAssetResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SellAssetResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
