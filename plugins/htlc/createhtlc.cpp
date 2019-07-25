#include "include/createhtlc.hpp"

#include <panels/commands.hpp>

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
   const auto _claim_period_seconds = to_string(p_GWallet->panels.p_commands->DoDateToSeconds(date, time));
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   if(!p_GWallet->panels.p_commands->ValidateAccount(destination).valid())
      return;

   stringstream command;
   command << "htlc_create " << _source << " " << _destination << " " << _amount << " " << _asset_symbol
           << " " << _hash_algorithm << " " << _preimage_hash << " " << _preimage_size << " " << _claim_period_seconds
           << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _source,
         _("Confirm HTLC Create?"), _cli, _broadcast);

   if(!response.IsNull())
      new CreateHtlcResponse(p_GWallet, response);
}

CreateHtlcResponse::CreateHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC Create response"), true);
}
