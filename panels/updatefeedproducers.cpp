#include "../include/panels/updatefeedproducers.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

UpdateAssetFeedProducers::UpdateAssetFeedProducers(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   new_feed_producers->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UpdateAssetFeedProducers::OnOk));
}

void UpdateAssetFeedProducers::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _symbol = symbol->GetValue().ToStdString();
   const auto _new_feed_producers = new_feed_producers->GetValue().ToStdString();

   flat_set<string> _new_feed_producers_set;
   try
   {
      auto _new_feed_producers_variant = fc::json::from_string(_new_feed_producers);
      _new_feed_producers_set = _new_feed_producers_variant.as<flat_set<string>>(2);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("List is invalid"));
      new_feed_producers->SetFocus();
      return;
   }

   signed_transaction result_obj;
   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   if(!p_GWallet->panels.p_commands->ValidateAsset(symbol).valid())
      return;

   wxAny response;
   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "update_asset_feed_producers " + _symbol + " " + _new_feed_producers + " " + _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->update_asset_feed_producers(_symbol,
               _new_feed_producers_set, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]),
                  _("Confirm update of feed producers?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->update_asset_feed_producers(_symbol,
                     _new_feed_producers_set, true);
               p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
            }
            response = result_obj;
            new UpdateAssetFeedProducersResponse(p_GWallet, response);
         }
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

UpdateAssetFeedProducersResponse::UpdateAssetFeedProducersResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Update feed producers response"), true);
}
