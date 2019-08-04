#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class BitAssetOptions : public wxDialog {
protected:
   GWallet* p_GWallet;

   wxSpinCtrl* feed_lifetime_min;
   wxSpinCtrl* minimum_feeds;
   wxSpinCtrl* force_settlement_delay_min;
   wxSpinCtrl* force_settlement_offset_percent;
   wxTextCtrl* maximum_force_settlement_volume;
   wxSearchCtrl* short_backing_asset;

   wxString options;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("BitAssetOptions"), wxT("wxDialog"));
      feed_lifetime_min = XRCCTRL(*this,"feed_lifetime_min",wxSpinCtrl);
      minimum_feeds = XRCCTRL(*this,"minimum_feeds",wxSpinCtrl);
      force_settlement_delay_min = XRCCTRL(*this,"force_settlement_delay_min",wxSpinCtrl);
      force_settlement_offset_percent = XRCCTRL(*this,"force_settlement_offset_percent",wxSpinCtrl);
      maximum_force_settlement_volume = XRCCTRL(*this,"maximum_force_settlement_volume",wxTextCtrl);
      short_backing_asset = XRCCTRL(*this,"short_backing_asset",wxSearchCtrl);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent &event);

public:
   BitAssetOptions(GWallet* gwallet);
   wxString GetOptions();
};
