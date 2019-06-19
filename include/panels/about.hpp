#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class About : public wxScrolledWindow
{
public:
   About(GWallet* gwallet);
   GWallet* p_GWallet;

protected:
   wxStaticText* client_version;
   wxStaticText* graphene_revision;
   wxStaticText* revision_age;
   wxStaticText* fc_revision;
   wxStaticText* fc_revision_age;
   wxStaticText* compile_date;
   wxStaticText* boost_version;
   wxStaticText* openssl_version;
   wxStaticText* os_build;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("About"), wxT("wxScrolledWindow"));
      client_version = XRCCTRL(*this,"client_version",wxStaticText);
      graphene_revision = XRCCTRL(*this,"graphene_revision",wxStaticText);
      revision_age = XRCCTRL(*this,"revision_age",wxStaticText);
      fc_revision = XRCCTRL(*this,"fc_revision",wxStaticText);
      fc_revision_age = XRCCTRL(*this,"fc_revision_age",wxStaticText);
      compile_date = XRCCTRL(*this,"compile_date",wxStaticText);
      boost_version = XRCCTRL(*this,"boost_version",wxStaticText);
      openssl_version = XRCCTRL(*this,"openssl_version",wxStaticText);
      os_build = XRCCTRL(*this,"os_build",wxStaticText);
   }
};
