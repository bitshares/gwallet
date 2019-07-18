#include "../include/panels/sellasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

SellAsset::SellAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   sell_asset->Append(p_GWallet->strings.assets);
   sell_asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   sell_amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);
   receive_amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SellAsset::OnOk));
   Connect(XRCID("receive_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SellAsset::OnSearchAsset), NULL, this);

   SetScrollRate(1,1);
}

void SellAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *receive_asset);
}

void SellAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _seller = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto _sell_amount = sell_amount->GetValue().ToStdString();
   const auto _sell_asset = p_GWallet->strings.assets[sell_asset->GetCurrentSelection()].ToStdString();
   const auto _receive_amount = receive_amount->GetValue().ToStdString();
   const auto _receive_asset = receive_asset->GetValue().ToStdString();
   const auto _fill_or_kill = fill_or_kill->GetValue() ? true : false;
   const auto _expiration = to_string(p_GWallet->panels.p_commands->DoDateToSeconds(date, time));
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   if(!p_GWallet->panels.p_commands->ValidateAsset(receive_asset).valid())
      return;

   stringstream command;
   command << "sell_asset " << _seller << " " << _sell_amount << " " << _sell_asset << " " << _receive_amount
           << " " << _receive_asset << " " << _expiration << " " << std::boolalpha << _fill_or_kill
           << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _seller,
         _("Confirm Sell Asset?"), _cli, _broadcast);

   if(!response.IsNull())
      new SellAssetResponse(p_GWallet, response);
}

SellAssetResponse::SellAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Sell asset response"), true);
}
