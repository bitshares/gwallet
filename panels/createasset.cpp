#include "../include/panels/createasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

CreateAsset::CreateAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   issuer->Append(p_GWallet->strings.accounts);
   issuer->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateAsset::OnOk));
}

void CreateAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto issuer_value = p_GWallet->strings.accounts[issuer->GetCurrentSelection()].ToStdString();
   const auto symbol_value = symbol->GetValue().ToStdString();
   const auto precision_value = precision->GetValue();
   const auto common_value = common->GetValue().ToStdString();
   const auto bitasset_opts_value = bitasset_opts->GetValue().ToStdString();
   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "create_asset " + issuer_value + " " + symbol_value + " " + to_string(precision_value) + " " +
            common_value + " " + bitasset_opts_value + " " + broadcast_value;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(issuer_value);
   }
   else
   {
      try {
         auto common_variant = fc::json::from_string(common_value);
         asset_options ao = common_variant.as<asset_options>(FC_PACK_MAX_DEPTH);

         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->create_asset(issuer_value, symbol_value, precision_value,
               ao, {}, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm transfer?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->create_asset(issuer_value, symbol_value, precision_value,
                     ao, {}, true);
               p_GWallet->DoAssets(issuer_value);
            }
         }
         response = result_obj;
         new CreateAssetResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

CreateAssetResponse::CreateAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Create asset response"), true);
}
