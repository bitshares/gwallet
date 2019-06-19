#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class Transfer : public wxScrolledWindow {
protected:
   GWallet* p_GWallet;

   wxComboBox* from;
   wxSearchCtrl* to;
   wxTextCtrl* amount;
   wxComboBox* asset;
   wxTextCtrl* memo;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Transfer"), wxT("wxScrolledWindow"));
      from = XRCCTRL(*this,"from",wxComboBox);
      to = XRCCTRL(*this,"to",wxSearchCtrl);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      asset = XRCCTRL(*this,"asset",wxComboBox);
      memo = XRCCTRL(*this,"memo",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent &event);

public:
   Transfer(GWallet* gwallet);
};

class TransferResponse: public wxScrolledWindow
{
public:
   TransferResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
