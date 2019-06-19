#include "../include/panels/commands.hpp"
#include "../include/panels/wallet.hpp"

Commands::Commands(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);
}