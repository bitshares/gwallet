#include "include/transfer.hpp"

#include <panels/commands.hpp>

Transfer::Transfer(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/assets/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   //to->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   asset->Append(p_GWallet->strings.assets);
   asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Transfer::OnOk));
   Connect(XRCID("to"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(Transfer::OnSearchAccount), NULL, this);

   SetScrollRate(1,1);
}

void Transfer::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *to);
}

void Transfer::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _from = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto _to = to->GetValue().ToStdString();
   const auto _amount = amount->GetValue().ToStdString();
   const auto _asset = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();
   const auto _memo = memo->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   if(!p_GWallet->panels.p_commands->ValidateAccount(to).valid())
      return;

   stringstream command;
   command << "transfer " << _from << " " << _to << " " << _amount << " " << _asset
           << " \"" << _memo << "\" " << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _from,
         _("Confirm transfer?"), _cli, _broadcast);

   if(!response.IsNull())
      new TransferResponse(p_GWallet, response);
}

TransferResponse::TransferResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Transfer response"), true);
}
