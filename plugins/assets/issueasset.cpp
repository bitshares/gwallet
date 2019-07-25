#include "include/issueasset.hpp"

#include <panels/commands.hpp>

IssueAsset::IssueAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);
   amount->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

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
   if(!Validate())
      return;

   const auto _to_account = to_account->GetValue().ToStdString();
   const auto _amount = amount->GetValue().ToStdString();
   const auto _symbol = symbol->GetValue().ToStdString();
   const auto _memo = memo->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   if(!p_GWallet->panels.p_commands->ValidateAccount(to_account).valid())
      return;

   stringstream command;
   command << "issue_asset " << _to_account << " " << _amount << " " << _symbol << " \"" << _memo
           << "\" " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(),
         p_GWallet->strings.selected_account.ToStdString(), _("Confirm issue asset?"), _cli, _broadcast);

   if(!response.IsNull())
      new IssueAssetResponse(p_GWallet, response);
}

IssueAssetResponse::IssueAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Issue asset response"), true);
}
