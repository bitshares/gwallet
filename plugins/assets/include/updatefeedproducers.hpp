#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class UpdateAssetFeedProducers: public wxScrolledWindow
{
public:
   UpdateAssetFeedProducers(GWallet* gwallet);

protected:
   wxSearchCtrl* symbol;
   wxTextCtrl* new_feed_producers;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("UpdateAssetFeedProducers"), wxT("wxScrolledWindow"));
      symbol = XRCCTRL(*this,"symbol",wxSearchCtrl);
      new_feed_producers = XRCCTRL(*this,"new_feed_producers",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class UpdateAssetFeedProducersResponse: public wxScrolledWindow
{
public:
   UpdateAssetFeedProducersResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("UpdateAssetFeedProducersResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};