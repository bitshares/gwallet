#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class ActiveCommittee : public wxScrolledWindow
{
public:
   ActiveCommittee(GWallet* gwallet);
   GWallet* p_GWallet;

protected:
   wxGrid* response_grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ActiveCommittee"), wxT("wxScrolledWindow"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};