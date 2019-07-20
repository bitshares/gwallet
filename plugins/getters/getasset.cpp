#include "include/getasset.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetAsset::GetAsset(GWallet* gwallet)
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/getters/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetAsset::OnOk));
   Connect(XRCID("asset_name_or_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetAsset::OnSearchAsset), NULL, this);
}

void GetAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *asset_name_or_id);
}

void GetAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _asset_name_or_id = asset_name_or_id->GetValue().ToStdString();
   asset_object result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   auto validate = p_GWallet->panels.p_commands->ValidateAsset(asset_name_or_id);
   if(!validate.valid())
      return;
   response = *validate;

   new GetAssetResponse(p_GWallet, response);

   if(cli->IsChecked())
   {
      auto command = "get_asset " + _asset_name_or_id;
      p_GWallet->panels.p_cli->DoCommand(command);
   }
}

GetAssetResponse::GetAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   asset_object result = any_response.As<asset_object>();

   // Todo: Too much code just to have the benefit of keys of objects translated
   // Probably better to loop throw the object and build the tree

   const auto root = response_tree->AddRoot(_("Asset object"));

   const auto id = response_tree->AppendItem(root, "ID");
   response_tree->AppendItem(id, string(object_id_type(result.id)));

   const auto symbol = response_tree->AppendItem(root, _("Symbol"));
   response_tree->AppendItem(symbol, result.symbol);

   const auto precision = response_tree->AppendItem(root, _("Precision"));
   response_tree->AppendItem(precision, to_string(result.precision));

   const auto issuer = response_tree->AppendItem(root, _("Issuer"));
   response_tree->AppendItem(issuer, string(object_id_type(result.issuer)));

   const auto options = response_tree->AppendItem(root, "Options");

   const auto max_supply = response_tree->AppendItem(options, _("Max supply"));
   response_tree->AppendItem(max_supply, wxNumberFormatter::ToString(result.options.max_supply.value));

   const auto market_fee_percent = response_tree->AppendItem(options, _("Market fee percent"));
   response_tree->AppendItem(market_fee_percent, to_string(result.options.market_fee_percent));

   const auto max_market_fee = response_tree->AppendItem(options, _("Max market fee"));
   response_tree->AppendItem(max_market_fee, wxNumberFormatter::ToString(result.options.max_market_fee.value));

   const auto issuer_permissions = response_tree->AppendItem(options, _("Issuer permissions"));
   response_tree->AppendItem(issuer_permissions, to_string(result.options.issuer_permissions));

   const auto flags = response_tree->AppendItem(options, _("Flags"));
   response_tree->AppendItem(flags, to_string(result.options.flags));

   const auto core_exchange_rate = response_tree->AppendItem(options, "Core exchange rate");
   const auto base = response_tree->AppendItem(core_exchange_rate, _("Base"));
   const auto base_amount = response_tree->AppendItem(base, _("Amount"));
   response_tree->AppendItem(base_amount, wxNumberFormatter::ToString(result.options.core_exchange_rate.base.amount.value));
   const auto base_asset_id = response_tree->AppendItem(base, _("Asset ID"));
   response_tree->AppendItem(base_asset_id, string(object_id_type(result.options.core_exchange_rate.base.asset_id)));
   const auto quote = response_tree->AppendItem(core_exchange_rate, _("Quote"));
   const auto quote_amount = response_tree->AppendItem(quote, _("Amount"));
   response_tree->AppendItem(quote_amount, wxNumberFormatter::ToString(result.options.core_exchange_rate.quote.amount.value));
   const auto quote_asset_id = response_tree->AppendItem(quote, _("Asset ID"));
   response_tree->AppendItem(quote_asset_id, string(object_id_type(result.options.core_exchange_rate.quote.asset_id)));

   const auto whitelist_authorities = response_tree->AppendItem(options, "Whitelist authorities");
   response_tree->AppendItem(whitelist_authorities, fc::json::to_string(result.options.whitelist_authorities));
   const auto blacklist_authorities = response_tree->AppendItem(options, "Blacklist authorities");
   response_tree->AppendItem(blacklist_authorities, fc::json::to_string(result.options.blacklist_authorities));
   const auto whitelist_markets = response_tree->AppendItem(options, "Whitelist markets");
   response_tree->AppendItem(whitelist_markets, fc::json::to_string(result.options.whitelist_markets));
   const auto blacklist_markets = response_tree->AppendItem(options, "Blacklist markets");
   response_tree->AppendItem(blacklist_markets, fc::json::to_string(result.options.blacklist_markets));

   const auto description = response_tree->AppendItem(options, _("Description"));
   response_tree->AppendItem(description, result.options.description);

   const auto extensions = response_tree->AppendItem(options, "Extensions");
   response_tree->AppendItem(extensions, fc::json::to_string(result.options.extensions));

   const auto dynamic_asset_data_id = response_tree->AppendItem(root, "Dynamic asset data ID");
   response_tree->AppendItem(dynamic_asset_data_id, fc::json::to_string(result.dynamic_asset_data_id));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, _("Get asset response"), true);
}
