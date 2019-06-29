#include "../include/panels/createhtlc.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

CreateHtlc::CreateHtlc(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   source->Append(p_GWallet->strings.accounts);
   source->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   asset_symbol->Append(p_GWallet->strings.assets);
   asset_symbol->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateHtlc::OnOk));
   Connect(XRCID("destination"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(CreateHtlc::OnSearchAccount), NULL, this);

   SetScrollRate(1,1);
}

void CreateHtlc::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *destination);
}

void CreateHtlc::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto source_value = p_GWallet->strings.accounts[source->GetCurrentSelection()].ToStdString();
   const auto destination_value = destination->GetValue().ToStdString();
   const auto amount_value = amount->GetValue().ToStdString();
   const auto asset_symbol_value = p_GWallet->strings.assets[asset_symbol->GetCurrentSelection()].ToStdString();
   const auto hash_algorithm_value = hash_algorithm->GetString(hash_algorithm->GetSelection()).ToStdString();
   const auto preimage_hash_value = preimage_hash->GetValue().ToStdString();
   const auto preimage_size_value = preimage_size->GetValue().ToStdString();
   uint32_t claim_period_seconds_value = p_GWallet->panels.p_commands->DoDateToSeconds(date, time);

   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "htlc_create " + source_value + " " + destination_value + " " + amount_value + " " + asset_symbol_value +
            " " + hash_algorithm_value + " " + preimage_hash_value + " " + preimage_size_value + " " +
            to_string(claim_period_seconds_value) + " " + broadcast_value;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(source_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_create(source_value, destination_value, amount_value,
               asset_symbol_value, hash_algorithm_value, preimage_hash_value, std::atoi(preimage_size_value.c_str()),
               claim_period_seconds_value, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm HTLC Create?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_create(source_value, destination_value, amount_value,
                     asset_symbol_value, hash_algorithm_value, preimage_hash_value, std::atoi(preimage_size_value.c_str()),
                     claim_period_seconds_value, true);
               p_GWallet->DoAssets(source_value);
            }
         }
         response = result_obj;
         new CreateHtlcResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

CreateHtlcResponse::CreateHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC Create response"), true);
}
