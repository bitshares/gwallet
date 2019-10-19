#include "include/transfertoblind.hpp"
#include "../include/panels/cli.hpp"

#include <panels/commands.hpp>

TransferToBlind::TransferToBlind(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   amount->SetValidator(*p_GWallet->panels.p_commands->numeric_validator);

   asset->Append(p_GWallet->strings.assets);
   asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   stringstream command;
   command << "get_my_blind_accounts";

   auto response =
         p_GWallet->panels.p_commands->ExecuteGetterCommand < map < string, public_key_type>>(command.str(),
               false, _("No blind accounts"));

   wxArrayString labels;
   if (!response.IsNull()) {
      auto results = response.As<map<string, public_key_type>>();
      auto z = 0;
      for (auto &blind : results) {

         labels.push_back(blind.first);
         ++z;
      }
   }
   to->Append(labels);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TransferToBlind::OnOk));
}

void TransferToBlind::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _from = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto _to = to->GetStringSelection();
   const auto _amount = amount->GetValue().ToStdString();
   const auto _asset = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();

   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   vector<pair<string, string>> to_amounts;
   pair<string, string> p = pair<string, string>(_to, _amount);
   to_amounts.push_back(p);

   stringstream command;
   command << "transfer_to_blind " << _from << " \"" << _asset << "\" " << fc::json::to_string(to_amounts)
         << " " << std::boolalpha << _broadcast;

   if(_cli)
   {
      p_GWallet->panels.p_cli->DoCommand(command.str());
      return;
   }
   else {
      const fc::variants line_variants = fc::json::variants_from_string(command.str());
      const auto command_name = line_variants[0].get_string();
      auto arguments_variants = fc::variants(line_variants.begin() + 1, line_variants.end());

      try {
         const auto result_obj = p_GWallet->bitshares.wallet_cli->receive_call(p_GWallet->bitshares.api_id, command_name,
               arguments_variants);

         const auto blind_confirm = result_obj.as<graphene::wallet::blind_confirmation>(GRAPHENE_MAX_NESTED_OBJECTS);

         new TransferToBlindResponse(p_GWallet, blind_confirm);
      }
      catch (fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         return;
      }
   }
}

TransferToBlindResponse::TransferToBlindResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<graphene::wallet::blind_confirmation>().trx);
   gwallet->panels.p_commands->notebook->AddPage(this, _("Transfer to blind response"), true);
}
