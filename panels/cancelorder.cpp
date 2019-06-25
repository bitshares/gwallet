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

      p_GWallet->panels.p_commands->Wait();

      if(cli->IsChecked())
      {
         auto command = "cancel_order " + std::string(object_id_type(order_id)) + " " + broadcast_v;
         p_GWallet->panels.p_cli->DoCommand(command);
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
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, "Cancel order response", true);
}
