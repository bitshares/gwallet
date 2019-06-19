#include "include/main.hpp"
#include "include/gwallet.hpp"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
   GWallet *gwallet = new GWallet(_("G-Wallet | Secure Bitshares Wallet"));
   gwallet->Show(true);

   return true;
}

