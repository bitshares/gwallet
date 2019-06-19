#include "../include/panels/sellasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

SellAsset::SellAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   sell_asset->Append(p_GWallet->strings.assets);
   sell_asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SellAsset::OnOk));
   Connect(XRCID("receive_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SellAsset::OnSearchAsset), NULL, this);

   SetScrollRate(1,1);
}

void SellAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *receive_asset);
}

void SellAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_value = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto sell_amount_value = sell_amount->GetValue().ToStdString();
   const auto sell_asset_value = p_GWallet->strings.assets[sell_asset->GetCurrentSelection()].ToStdString();
   const auto receive_amount_value = receive_amount->GetValue().ToStdString();
   const auto receive_asset_value = receive_asset->GetValue().ToStdString();
   const auto fill_or_kill_value = fill_or_kill->GetValue() ? true : false;

   const auto date_value = date->GetValue().ToUTC().GetValue().ToLong()/1000;
   const auto time_second = time->GetValue().ToUTC().GetSecond();
   const auto time_minute = time->GetValue().ToUTC().GetMinute();
   const auto time_hour = time->GetValue().ToUTC().GetHour();
   const auto time_value = (time_hour*3600) + (time_minute*60) + time_second;
   auto now = wxDateTime::Now().ToUTC().GetValue().ToLong()/1000;
   uint32_t expiration_value = date_value + time_value - now;
   string broadcast_v = "false";
   if(broadcast->IsChecked())
      broadcast_v = "true";
   string fill_or_kill_v = "false";
   if(fill_or_kill->IsChecked())
      fill_or_kill_v = "true";

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(receive_asset_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Asset is invalid"));
      receive_asset->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "sell_asset " + seller_value + " " + sell_amount_value + " " + sell_asset_value + " " +
            receive_amount_value + " " + receive_asset_value + " " + to_string(expiration_value) + " " +
            " " + fill_or_kill_v + " " + broadcast_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(seller_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_value, sell_amount_value,
               sell_asset_value, receive_amount_value, receive_asset_value, expiration_value, fill_or_kill_value, false);
         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm Sell Asset?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_value, sell_amount_value,
                     sell_asset_value, receive_amount_value, receive_asset_value, expiration_value, fill_or_kill_value, true);
               p_GWallet->DoAssets(seller_value);
            }
         }
         response = result_obj;
         new SellAssetResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

SellAssetResponse::SellAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   signed_transaction result = any_response.As<signed_transaction>();

   const auto root = response_tree->AddRoot("Signed Transaction");

   const auto ref_block_num = response_tree->AppendItem(root, "Reference block number");
   response_tree->AppendItem(ref_block_num, to_string(result.ref_block_num));

   const auto ref_block_prefix = response_tree->AppendItem(root, "Reference block prefix");
   response_tree->AppendItem(ref_block_prefix, to_string(result.ref_block_prefix));

   const auto expiration = response_tree->AppendItem(root, "Expiration");
   response_tree->AppendItem(expiration, result.expiration.to_iso_string());

   const auto operations = response_tree->AppendItem(root, "Operations");
   response_tree->AppendItem(operations, fc::json::to_string(result.operations));

   const auto extensions = response_tree->AppendItem(root, "Extensions");
   response_tree->AppendItem(extensions, fc::json::to_string(result.extensions));

   const auto signatures = response_tree->AppendItem(root, "Signatures");
   response_tree->AppendItem(signatures, fc::json::to_string(result.signatures));

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, "Sell asset response");
}
