#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class CreateBlindAccount: public wxScrolledWindow
{
public:
   CreateBlindAccount(GWallet* gwallet);

protected:
   wxTextCtrl* label;
   wxTextCtrl* brain_key;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateBlindAccount"), wxT("wxScrolledWindow"));
      label = XRCCTRL(*this,"label",wxTextCtrl);
      brain_key = XRCCTRL(*this,"brain_key",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnGetBrainKey(wxCommandEvent &event);
};

class CreateBlindAccountResponse: public wxScrolledWindow
{
public:
   CreateBlindAccountResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CreateBlindAccountResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
