#include "../include/panels/about.hpp"
#include "../include/panels/wallet.hpp"

About::About(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   const auto about = p_GWallet->bitshares.wallet_api_ptr->about();

   client_version->SetLabel(about["client_version"].as_string());
   graphene_revision->SetLabel(about["graphene_revision"].as_string());
   revision_age->SetLabel(about["graphene_revision_age"].as_string());
   fc_revision->SetLabel(about["fc_revision"].as_string());
   fc_revision_age->SetLabel(about["fc_revision_age"].as_string());
   compile_date->SetLabel(about["compile_date"].as_string());
   boost_version->SetLabel(about["boost_version"].as_string());
   openssl_version->SetLabel(about["openssl_version"].as_string());
   os_build->SetLabel(about["build"].as_string());
}
