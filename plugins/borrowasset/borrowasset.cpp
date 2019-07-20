#include "include/borrowasset.hpp"

#include <panels/commands.hpp>

BorrowAsset::BorrowAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/borrowasset/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   borrow_amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);
   //borrow_asset->SetValidator(*p_GWallet->panels.p_commands->empty_validator);
   collateral_amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BorrowAsset::OnOk));
   Connect(XRCID("borrow_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(BorrowAsset::OnSearchAsset), NULL, this);

   SetScrollRate(1,1);
}

void BorrowAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *borrow_asset);
}

void BorrowAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _seller = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto _borrow_amount = borrow_amount->GetValue().ToStdString();
   const auto _borrow_asset = borrow_asset->GetValue().ToStdString();
   const auto _collateral_amount = collateral_amount->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "borrow_asset " << _seller << " " << _borrow_amount << " " << _borrow_asset << " " << _collateral_amount
           << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _seller,
         _("Confirm Borrow Asset?"), _cli, _broadcast);

   if(!response.IsNull())
      new BorrowAssetResponse(p_GWallet, response);
}

BorrowAssetResponse::BorrowAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, "Borrow asset response", true);
}
