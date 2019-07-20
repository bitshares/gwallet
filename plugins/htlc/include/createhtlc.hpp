#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class CreateHtlc : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxComboBox* source;
   wxSearchCtrl* destination;
   wxTextCtrl* amount;
   wxComboBox* asset_symbol;
   wxChoice* hash_algorithm;
   wxTextCtrl* preimage_hash;
   wxTextCtrl* preimage_size;
   wxDatePickerCtrl* date;
   wxTimePickerCtrl* time;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateHtlc"), wxT("wxScrolledWindow"));
      source = XRCCTRL(*this,"source",wxComboBox);
      destination = XRCCTRL(*this,"destination",wxSearchCtrl);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      asset_symbol = XRCCTRL(*this,"asset_symbol",wxComboBox);
      hash_algorithm = XRCCTRL(*this,"hash_algorithm",wxChoice);
      preimage_hash = XRCCTRL(*this,"preimage_hash",wxTextCtrl);
      preimage_size = XRCCTRL(*this,"preimage_size",wxTextCtrl);
      date = XRCCTRL(*this,"date",wxDatePickerCtrl);
      time = XRCCTRL(*this,"time",wxTimePickerCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);

public:
   CreateHtlc(GWallet* gwallet);
};

class CreateHtlcResponse: public wxScrolledWindow
{
public:
   CreateHtlcResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateHtlcResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
