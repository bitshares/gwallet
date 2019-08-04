#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class AssetOptions : public wxDialog {
protected:
   GWallet* p_GWallet;

   wxTextCtrl* max_supply;
   wxSpinCtrl* market_fee_percent;
   wxTextCtrl* max_market_fee;
   wxTextCtrl* issuer_permissions;
   wxTextCtrl* flags;
   wxTextCtrl* core_exchange_rate;
   wxListCtrl* whitelist_authorities;
   wxListCtrl* blacklist_authorities;
   wxListCtrl* whitelist_markets;
   wxListCtrl* blacklist_markets;
   wxTextCtrl* description;

   wxString options;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("AssetOptions"), wxT("wxDialog"));
      max_supply = XRCCTRL(*this,"max_supply",wxTextCtrl);
      market_fee_percent = XRCCTRL(*this,"market_fee_percent",wxSpinCtrl);
      max_market_fee = XRCCTRL(*this,"max_market_fee",wxTextCtrl);
      issuer_permissions = XRCCTRL(*this,"issuer_permissions",wxTextCtrl);
      flags = XRCCTRL(*this,"flags",wxTextCtrl);
      core_exchange_rate = XRCCTRL(*this,"core_exchange_rate",wxTextCtrl);
      whitelist_authorities = XRCCTRL(*this,"whitelist_authorities",wxListCtrl);
      blacklist_authorities = XRCCTRL(*this,"blacklist_authorities",wxListCtrl);
      whitelist_markets = XRCCTRL(*this,"whitelist_markets",wxListCtrl);
      blacklist_markets = XRCCTRL(*this,"blacklist_markets",wxListCtrl);
      description = XRCCTRL(*this,"description",wxTextCtrl);
   }
   void OnOk(wxCommandEvent& event);

public:
   AssetOptions(GWallet* gwallet);
   wxString GetOptions();
};
