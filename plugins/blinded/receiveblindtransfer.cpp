#include "include/receiveblindtransfer.hpp"
#include "../include/panels/cli.hpp"

#include <panels/commands.hpp>

ReceiveBlindTransfer::ReceiveBlindTransfer(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ReceiveBlindTransfer::OnOk));
}

void ReceiveBlindTransfer::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _confirmation_receipt = confirmation_receipt->GetValue().ToStdString();
   const auto _opt_from = opt_from->GetValue().ToStdString();
   const auto _opt_memo = opt_memo->GetValue().ToStdString();

   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   std::stringstream command;
   command << "receive_blind_transfer " << _confirmation_receipt << " \"" << _opt_from << "\" \"" << _opt_memo << "\"";

   if(_cli)
   {
      p_GWallet->panels.p_cli->DoCommand(command.str());
      return;
   }
   else {
      const fc::variants line_variants = fc::json::variants_from_string(command.str());
      const auto command_name = line_variants[0].get_string();
      auto arguments_variants = fc::variants(line_variants.begin() + 1, line_variants.end());

      try {
         const auto result_obj = p_GWallet->bitshares.wallet_cli->receive_call(p_GWallet->bitshares.api_id, command_name,
               arguments_variants);

         const auto blind_confirm = result_obj.as<graphene::wallet::blind_confirmation>(GRAPHENE_MAX_NESTED_OBJECTS);

         new ReceiveBlindTransferResponse(p_GWallet, blind_confirm);
      }
      catch (fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         return;
      }
   }
}

ReceiveBlindTransferResponse::ReceiveBlindTransferResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<graphene::wallet::blind_confirmation>().trx);
   gwallet->panels.p_commands->notebook->AddPage(this, _("Receive blind transfer response"), true);
}
