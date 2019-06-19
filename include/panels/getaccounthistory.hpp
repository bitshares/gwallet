#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetAccountHistory: public wxScrolledWindow
{
public:
   GetAccountHistory(GWallet* gwallet);

protected:
   wxSearchCtrl* name;
   wxSpinCtrl* limit;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccountHistory"), wxT("wxScrolledWindow"));
      name = XRCCTRL(*this,"name",wxSearchCtrl);
      limit = XRCCTRL(*this,"limit",wxSpinCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetAccountHistoryResponse: public wxScrolledWindow
{
public:
   GetAccountHistoryResponse(GWallet* gwallet, wxAny any_response);
   wxGrid* response_grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccountHistoryResponse"), wxT("wxScrolledWindow"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};