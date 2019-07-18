#include <wx/wx.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/richmsgdlg.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Commands : public wxPanel
{
public:
   Commands(GWallet* gwallet);

   void DoSignedTranactionResponse(wxTreeCtrl* response_tree, signed_transaction result);
   void Wait();
   void DoGridProperties(wxGrid* grid);
   uint32_t DoDateToSeconds(wxDatePickerCtrl* date, wxTimePickerCtrl* time);
   optional<wxAny> ValidateAccount(wxSearchCtrl* control);
   optional<wxAny> ValidateAsset(wxSearchCtrl* control);

   wxAny ExecuteWalletCommand(string command_string, string account, wxString confirm, bool cli, bool broadcast);

   Commands(wxWindow *parent=NULL){
      InitWidgetsFromXRC((wxWindow *)parent);
   }
   GWallet* p_GWallet;
   wxAuiNotebook* notebook;

   wxTextValidator* numeric_validator = new wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC);
   wxTextValidator* empty_validator = new wxTextValidator(wxFILTER_EMPTY);

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->AddHandler(new wxAuiNotebookXmlHandler);
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Commands"), wxT("wxPanel"));
      notebook = XRCCTRL(*this,"notebook",wxAuiNotebook);
   }
};
