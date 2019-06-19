#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include "../gwallet.hpp"

class CancelOrder: public wxScrolledWindow
{
public:
   CancelOrder(GWallet* gwallet);

protected:
   wxListBox* order;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   wxArrayString open_orders_strings;
   vector<object_id_type> open_orders_ids;

   void OnOk(wxCommandEvent& event);
   void DoOpenOrders();

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CancelOrder"), wxT("wxScrolledWindow"));
      order = XRCCTRL(*this,"order",wxListBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class CancelOrderResponse: public wxScrolledWindow
{
public:
   CancelOrderResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CancelOrderResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
