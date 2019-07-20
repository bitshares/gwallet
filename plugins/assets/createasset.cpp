#include "include/createasset.hpp"
#include "include/assetoptions.hpp"

#include <panels/commands.hpp>

CreateAsset::CreateAsset(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/assets/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   issuer->Append(p_GWallet->strings.accounts);
   issuer->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   symbol->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   common->SetValue(wxT("{}"));
   common->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   bitasset_opts->SetValue(wxT("{}"));
   bitasset_opts->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateAsset::OnOk));
   Connect(XRCID("generate_common"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateAsset::OnGenerateCommon));
}

void CreateAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _issuer = p_GWallet->strings.accounts[issuer->GetCurrentSelection()].ToStdString();
   const auto _symbol = symbol->GetValue().ToStdString();
   const auto _precision = to_string(precision->GetValue());
   const auto _common = common->GetValue().ToStdString();
   const auto _bitasset_opts = bitasset_opts->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "create_asset " << _issuer << " " << _symbol << " " << _precision << " " << _common
           << " " << _bitasset_opts << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _issuer,
         _("Confirm create asset?"), _cli, _broadcast);

   if(!response.IsNull())
      new CreateAssetResponse(p_GWallet, response);
}

CreateAssetResponse::CreateAssetResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Create asset response"), true);
}

void CreateAsset::OnGenerateCommon(wxCommandEvent& WXUNUSED(event))
{
   AssetOptions AssetOptions(p_GWallet);
   common->SetValue(AssetOptions.GetOptions());
}
