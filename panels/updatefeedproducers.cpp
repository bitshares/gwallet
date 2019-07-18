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
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   flat_set<string> _new_feed_producers_set;
   try
   {
      auto _new_feed_producers_variant = fc::json::from_string(_new_feed_producers);
      _new_feed_producers_variant.as<flat_set<string>>(2);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("List is invalid"));
      new_feed_producers->SetFocus();
      return;
   }

   if(!p_GWallet->panels.p_commands->ValidateAsset(symbol).valid())
      return;

   stringstream command;
   command << "update_asset_feed_producers " << _symbol << " " << _new_feed_producers << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(),
         p_GWallet->strings.selected_account.ToStdString(), _("Confirm update of feed producers?"), _cli, _broadcast);

   if(!response.IsNull())
      new UpdateAssetFeedProducersResponse(p_GWallet, response);
}

UpdateAssetFeedProducersResponse::UpdateAssetFeedProducersResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Update feed producers response"), true);
}
