#include "../include/panels/issueasset.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

IssueAsset::IssueAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(IssueAsset::OnOk));
   Connect(XRCID("to_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(IssueAsset::OnSearchAccount), NULL, this);
   Connect(XRCID("symbol"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(IssueAsset::OnSearchAsset), NULL, this);

   SetScrollRate(1,1);
}

void IssueAsset::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *to_account);
}

void IssueAsset::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *symbol);
}

void IssueAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto to_account_value = to_account->GetValue().ToStdString();
   const auto amount_value = amount->GetValue().ToStdString();
   const auto symbol_value = symbol->GetValue().ToStdString();
   const auto memo_value = memo->GetValue().ToStdString();
   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "issue_asset " + to_account_value + " " + amount_value + " " + symbol_value +
            " \"" + memo_value + "\" " + broadcast_value;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->issue_asset(to_account_value, amount_value, symbol_value,
               memo_value, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm issue asset?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->issue_asset(to_account_value, amount_value, symbol_value,
                     memo_value, true);
               p_GWallet->DoAssets(p_GWallet->strings.selected_account.ToStdString());
            }
         }
         response = result_obj;
         new IssueAssetResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

IssueAssetResponse::IssueAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Issue asset response"), true);
}
