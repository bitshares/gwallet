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

   //destination->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   asset_symbol->Append(p_GWallet->strings.assets);
   asset_symbol->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);
   preimage_size->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);
   preimage_hash->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

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
   if(!Validate())
      return;

   const auto _source = p_GWallet->strings.accounts[source->GetCurrentSelection()].ToStdString();
   const auto _destination = destination->GetValue().ToStdString();
   const auto _amount = amount->GetValue().ToStdString();
   const auto _asset_symbol = p_GWallet->strings.assets[asset_symbol->GetCurrentSelection()].ToStdString();
   const auto _hash_algorithm = hash_algorithm->GetString(hash_algorithm->GetSelection()).ToStdString();
   const auto _preimage_hash = preimage_hash->GetValue().ToStdString();
   const auto _preimage_size = preimage_size->GetValue().ToStdString();
   uint32_t _claim_period_seconds = p_GWallet->panels.p_commands->DoDateToSeconds(date, time);

   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   if(!p_GWallet->panels.p_commands->ValidateAccount(destination).valid())
      return;

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "htlc_create " + _source + " " + _destination + " " + _amount + " " + _asset_symbol +
            " " + _hash_algorithm + " " + _preimage_hash + " " + _preimage_size + " " +
            to_string(_claim_period_seconds) + " " + _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(_source);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_create(_source, _destination, _amount,
               _asset_symbol, _hash_algorithm, _preimage_hash, std::atoi(_preimage_size.c_str()),
               _claim_period_seconds, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm HTLC Create?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->htlc_create(_source, _destination, _amount,
                     _asset_symbol, _hash_algorithm, _preimage_hash, std::atoi(_preimage_size.c_str()),
                     _claim_period_seconds, true);
               p_GWallet->DoAssets(_source);
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
