#include "include/bitassetoptions.hpp"

#include <panels/commands.hpp>

BitAssetOptions::BitAssetOptions(GWallet* gwallet) : wxDialog()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   feed_lifetime_min->SetValue(to_string(GRAPHENE_MAX_SHARE_SUPPLY/60));
   minimum_feeds->SetValue("1");
   force_settlement_delay_min->SetValue(to_string(GRAPHENE_DEFAULT_FORCE_SETTLEMENT_DELAY/60));
   force_settlement_offset_percent->SetValue(to_string(GRAPHENE_DEFAULT_FORCE_SETTLEMENT_OFFSET));
   maximum_force_settlement_volume->SetValue(to_string(GRAPHENE_DEFAULT_FORCE_SETTLEMENT_MAX_VOLUME));
   maximum_force_settlement_volume->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BitAssetOptions::OnOk));
   Connect(XRCID("short_backing_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN,
         wxCommandEventHandler(BitAssetOptions::OnSearchAsset), NULL, this);

   ShowModal();
}

void BitAssetOptions::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *short_backing_asset);
}

void BitAssetOptions::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   bitasset_options bao;
   bao.feed_lifetime_sec = feed_lifetime_min->GetValue() * 60;
   bao.minimum_feeds = minimum_feeds->GetValue();
   bao.force_settlement_delay_sec = force_settlement_delay_min->GetValue() * 60;
   bao.force_settlement_offset_percent = force_settlement_offset_percent->GetValue();
   bao.maximum_force_settlement_volume = std::stol(maximum_force_settlement_volume->GetValue().ToStdString());

   bao.short_backing_asset = p_GWallet->bitshares.database_api->get_asset_id_from_string(
         short_backing_asset->GetValue().ToStdString());

   options = fc::json::to_string(bao);
   Close(true);
}

wxString BitAssetOptions::GetOptions()
{
   return options;
}
