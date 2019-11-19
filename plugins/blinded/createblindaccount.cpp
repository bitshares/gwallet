#include "include/createblindaccount.hpp"
#include "../include/panels/cli.hpp"

#include <panels/commands.hpp>

CreateBlindAccount::CreateBlindAccount(GWallet* gwallet)
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateBlindAccount::OnOk));
   Connect(XRCID("get_brain_key"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CreateBlindAccount::OnGetBrainKey), NULL, this);
}

void CreateBlindAccount::OnGetBrainKey(wxCommandEvent& event)
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   int key = wxMessageBox(result.brain_priv_key, _("Suggested Brain key"),
         wxNO_DEFAULT|wxOK|wxCANCEL|wxICON_INFORMATION, this);
   if (key == wxOK)
      brain_key->SetValue(result.brain_priv_key);
}

void CreateBlindAccount::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _label = label->GetValue().ToStdString();
   const auto _brain_key = brain_key->GetValue().ToStdString();

   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   std::stringstream command;
   command << "create_blind_account " << _label << " \"" << _brain_key << "\"";

   // Todo: We cant use ExecuteWalletCommand here
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
         auto result_obj = p_GWallet->bitshares.wallet_cli->receive_call(p_GWallet->bitshares.api_id, command_name,
               arguments_variants);

         wxMessageDialog dialog(NULL, _("Blinded account created"), _("Success"),
                                wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
         if (dialog.ShowModal() == wxID_OK)
            return;
      }
      catch (fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         return;
      }
   }
}

CreateBlindAccountResponse::CreateBlindAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Vote for witness response"), true);
}
