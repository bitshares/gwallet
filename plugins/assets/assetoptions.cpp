#include "include/assetoptions.hpp"

#include <panels/commands.hpp>

AssetOptions::AssetOptions(GWallet* gwallet) : wxDialog()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   max_supply->SetValue(to_string(GRAPHENE_MAX_SHARE_SUPPLY));
   max_supply->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   max_market_fee->SetValue(to_string(GRAPHENE_MAX_SHARE_SUPPLY));
   max_market_fee->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   issuer_permissions->SetValue(to_string(UIA_ASSET_ISSUER_PERMISSION_MASK));
   issuer_permissions->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   flags->SetValue(to_string(0));
   flags->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   price cer = price(asset(), asset(0, asset_id_type(1)));
   core_exchange_rate->SetValue(fc::json::to_string(cer));
   core_exchange_rate->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AssetOptions::OnOk));

   ShowModal();
}

void AssetOptions::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   asset_options ao;
   ao.max_supply = std::stol(max_supply->GetValue().ToStdString());
   ao.max_market_fee = std::stol(max_market_fee->GetValue().ToStdString());
   ao.issuer_permissions = std::stoi(issuer_permissions->GetValue().ToStdString());
   ao.flags = std::stoi(flags->GetValue().ToStdString());

   auto core_exchange_variant = fc::json::from_string(core_exchange_rate->GetValue().ToStdString());
   ao.core_exchange_rate = core_exchange_variant.as<price>(2);

   //ao.whitelist_authorities =
   //ao.blacklist_authorities =
   //ao.whitelist_markets =
   //ao.blacklist_markets =

   ao.description = description->GetValue().ToStdString();
   //ao.extensions =

   options = fc::json::to_string(ao);

   Close(true);
}

wxString AssetOptions::GetOptions()
{
   return options;
}