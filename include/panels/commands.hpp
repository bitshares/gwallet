#include <wx/wx.h>
#include <wx/treectrl.h>

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

   Commands(wxWindow *parent=NULL){
      InitWidgetsFromXRC((wxWindow *)parent);
   }
   GWallet* p_GWallet;
   wxAuiNotebook* notebook;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->AddHandler(new wxAuiNotebookXmlHandler);
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Commands"), wxT("wxPanel"));
      notebook = XRCCTRL(*this,"notebook",wxAuiNotebook);
   }
};
