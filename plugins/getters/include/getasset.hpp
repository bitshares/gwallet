#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

#include <panels/wallet.hpp>

class GetAsset: public wxScrolledWindow
{
public:
   GetAsset(GWallet* gwallet);

protected:
   wxSearchCtrl* asset_name_or_id;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAsset"), wxT("wxScrolledWindow"));
      asset_name_or_id = XRCCTRL(*this,"asset_name_or_id",wxSearchCtrl);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetAssetResponse: public wxScrolledWindow
{
public:
   GetAssetResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAssetResponse"), wxT("wxScrolledWindow"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};
