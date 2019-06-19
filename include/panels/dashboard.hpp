#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Dashboard : public wxPanel
{
public:
   Dashboard(GWallet* gwallet);

   Dashboard(wxWindow *parent=NULL){
      InitWidgetsFromXRC((wxWindow *)parent);
   }
   GWallet* p_GWallet;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Dashboard"), wxT("wxPanel"));
   }
};
