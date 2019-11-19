#include "include/cancelorder.hpp"

#include <panels/commands.hpp>

CancelOrder::CancelOrder(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   DoOpenOrders();
   order->Append(open_orders_strings);

   //order->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CancelOrder::OnOk));
}

void CancelOrder::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(order->IsEmpty()) {
      p_GWallet->OnError(this, _("No order selected"));
      return;
   }

   const auto _order_id = open_orders_ids[order->GetSelection()];
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   std::stringstream command;
   command << "cancel_order " << std::string(object_id_type(_order_id)) << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(),
         p_GWallet->strings.selected_account.ToStdString(), _("Confirm cancel order?"), _cli, _broadcast);

   if(!response.IsNull())
      new CancelOrderResponse(p_GWallet, response);
}

void CancelOrder::DoOpenOrders()
{
   auto full = p_GWallet->bitshares.wallet_api_ptr->get_full_account(p_GWallet->strings.selected_account.ToStdString());
   if(full.limit_orders.size() > 0) {
      for (auto &limit_order : full.limit_orders) {

         auto assets = p_GWallet->bitshares.database_api->get_assets({
            std::string(object_id_type(limit_order.amount_for_sale().asset_id)),
            std::string(object_id_type(limit_order.amount_to_receive().asset_id))
         }, false);

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
