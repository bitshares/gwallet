#include <wx/wx.h>

#include <panels/wallet.hpp>

class GetMyBlindAccounts : public wxScrolledWindow
{
public:
   GetMyBlindAccounts(GWallet* gwallet);

   GWallet* p_GWallet;

protected:
   wxGrid* response_grid;

private:

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetMyBlindAccounts"), wxT("wxScrolledWindow"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};
