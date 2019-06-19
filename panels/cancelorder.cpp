#include "../include/panels/cancelorder.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

CancelOrder::CancelOrder(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   DoOpenOrders();
   order->Append(open_orders_strings);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CancelOrder::OnOk));
}

void CancelOrder::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(order->IsEmpty()) {
      p_GWallet->OnError(this, _("No order selected"));
      return;
   }
   else {
      const auto order_id = open_orders_ids[order->GetSelection()];
      string broadcast_v = "false";
      if(broadcast->IsChecked())
         broadcast_v = "true";

      signed_transaction result_obj;
      wxAny response;

      wxBusyCursor wait;
      wxTheApp->Yield(true);

      if(cli->IsChecked())
      {
         auto command = "cancel_order " + std::string(object_id_type(order_id)) + " " + broadcast_v;
         p_GWallet->panels.p_cli->command->SetValue(command);
         wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
         p_GWallet->panels.p_cli->OnCliCommand(event);
         p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
      }
      else
      {
         try {
            auto result_obj = p_GWallet->bitshares.wallet_api_ptr->cancel_order(order_id, false);
            if(broadcast->IsChecked()) {
               if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm cancel order?"),
                     wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
                  wxTheApp->Yield(true);
                  result_obj = p_GWallet->bitshares.wallet_api_ptr->cancel_order(order_id, true);
                  p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
                  order->Delete(order->GetSelection());
               }
            }
            response = result_obj;
            new CancelOrderResponse(p_GWallet, response);
         }
         catch (const fc::exception &e) {
            p_GWallet->OnError(this, e.to_detail_string());
         }
      }
   }
}

void CancelOrder::DoOpenOrders()
{
   auto full = p_GWallet->bitshares.wallet_api_ptr->get_full_account(p_GWallet->strings.selected_account.ToStdString());
   if(full.limit_orders.size() > 0) {
      for (auto &limit_order : full.limit_orders) {

         auto assets = p_GWallet->bitshares.database_api->get_assets({
            std::string(object_id_type(limit_order.amount_for_sale().asset_id)),
            std::string(object_id_type(limit_order.amount_to_receive().asset_id))
         });

         auto pretty_balance_for_sale = p_GWallet->DoPrettyBalance(
               assets[0]->precision, limit_order.amount_for_sale().amount.value);
         auto pretty_balance_to_receive = p_GWallet->DoPrettyBalance(
               assets[1]->precision, limit_order.amount_to_receive().amount.value);

         open_orders_strings.Add(std::string(object_id_type(limit_order.id)) + " - Selling "+ pretty_balance_for_sale
               + " " + assets[0]->symbol + " for " + pretty_balance_to_receive + " " + assets[1]->symbol);
         open_orders_ids.push_back(limit_order.id);
      }
   }
}

CancelOrderResponse::CancelOrderResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   signed_transaction result = any_response.As<signed_transaction>();

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

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

   gwallet->panels.p_commands->notebook->AddPage(this, "Cancel order response");
}
