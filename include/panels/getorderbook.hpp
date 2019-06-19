#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/grid.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetOrderBook: public wxScrolledWindow
{
public:
   GetOrderBook(GWallet* gwallet);

protected:
   wxSearchCtrl* base;
   wxSearchCtrl* quote;
   wxSpinCtrl* limit;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetOrderBook"), wxT("wxScrolledWindow"));
      base = XRCCTRL(*this,"base",wxSearchCtrl);
      quote = XRCCTRL(*this,"quote",wxSearchCtrl);
      limit = XRCCTRL(*this,"limit",wxSpinCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetOrderBookResponse: public wxScrolledWindow
{
public:
   GetOrderBookResponse(GWallet* gwallet, wxAny any_response, std::string response_type);
   wxGrid* response_grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetOrderBookResponse"), wxT("wxScrolledWindow"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};
