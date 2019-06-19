#include "../include/panels/dashboard.hpp"
#include "../include/panels/wallet.hpp"

Dashboard::Dashboard(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);
}
