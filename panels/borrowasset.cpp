#include "../include/panels/borrowasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

BorrowAsset::BorrowAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
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
   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "borrow_asset " + _seller + " " + _borrow_amount + " " + _borrow_asset + " " +
            _collateral_amount + " " + _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(_seller);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(_seller, _borrow_amount,
               _borrow_asset, _collateral_amount, false);
         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm Borrow Asset?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(_seller, _borrow_amount,
                     _borrow_asset, _collateral_amount, true);
               p_GWallet->DoAssets(_seller);
            }
         }
         response = result_obj;
         new BorrowAssetResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

BorrowAssetResponse::BorrowAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, "Borrow asset response", true);
}
