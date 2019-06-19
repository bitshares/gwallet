#include "../include/panels/borrowasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

BorrowAsset::BorrowAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BorrowAsset::OnOk));
   Connect(XRCID("borrow_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(BorrowAsset::OnSearchAsset), NULL, this);

   SetScrollRate(1,1);
}

void BorrowAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *borrow_asset);
}

void BorrowAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_value = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto borrow_amount_value = borrow_amount->GetValue().ToStdString();
   const auto borrow_asset_value = borrow_asset->GetValue().ToStdString();
   const auto collateral_amount_value = collateral_amount->GetValue().ToStdString();
   string broadcast_v = "false";
   if(broadcast->IsChecked())
      broadcast_v = "true";

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(borrow_asset_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Asset is invalid"));
      borrow_asset->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "borrow_asset " + seller_value + " " + borrow_amount_value + " " + borrow_asset_value + " " +
            collateral_amount_value + " " + broadcast_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(seller_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
               borrow_asset_value, collateral_amount_value, false);
         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm Borrow Asset?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
                     borrow_asset_value, collateral_amount_value, true);
               p_GWallet->DoAssets(seller_value);
            }
         }
         response = result_obj;
         new BorrowAssetResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

BorrowAssetResponse::BorrowAssetResponse(GWallet* gwallet, wxAny any_response)
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

   gwallet->panels.p_commands->notebook->AddPage(this, "Borrow asset response");
}
