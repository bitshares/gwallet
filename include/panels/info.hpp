#include <wx/wx.h>
#include <wx/hyperlink.h>
#include <wx/numformatter.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Info : public wxScrolledWindow
{
public:
   Info(GWallet* gwallet);

   GWallet* p_GWallet;

protected:
   wxStaticText* head_block_num;
   wxStaticText* head_block_age;
   wxStaticText* next_maintenance_time;
   wxStaticText* participation;
   wxStaticText* chain_id;
   wxStaticText* core_asset;
   wxStaticText* blockchain_name;

private:

   void OnWitness(wxHyperlinkEvent& event);
   void OnCommittee(wxHyperlinkEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Info"), wxT("wxScrolledWindow"));
      head_block_num = XRCCTRL(*this,"head_block_num",wxStaticText);
      head_block_age = XRCCTRL(*this,"head_block_age",wxStaticText);
      next_maintenance_time = XRCCTRL(*this,"next_maintenance_time",wxStaticText);
      participation = XRCCTRL(*this,"participation",wxStaticText);
      chain_id = XRCCTRL(*this,"chain_id",wxStaticText);
      core_asset = XRCCTRL(*this,"core_asset",wxStaticText);
      blockchain_name = XRCCTRL(*this,"blockchain_name",wxStaticText);
   }
};
