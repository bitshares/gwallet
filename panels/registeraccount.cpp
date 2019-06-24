#include "../include/panels/registeraccount.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

RegisterAccount::RegisterAccount(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   registrar_account->Append(p_GWallet->strings.accounts);
   registrar_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RegisterAccount::OnOk));
   Connect(XRCID("referrer_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(RegisterAccount::OnSearchAccount), NULL, this);
}

void RegisterAccount::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *referrer_account);
}

void RegisterAccount::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto owner_value = owner->GetValue().ToStdString();
   const auto active_value = active->GetValue().ToStdString();
   const auto name_value = name->GetValue().ToStdString();
   const auto registrar_account_value = registrar_account->GetValue().ToStdString();
   const auto referrer_account_value = referrer_account->GetValue().ToStdString();
   const auto referrer_percent_value = referrer_percent->GetValue();

   string broadcast_value = "false";
   if(broadcast->IsChecked())
      broadcast_value = "true";

   signed_transaction result_obj;
   wxAny response;

   wxBusyCursor wait;
   wxTheApp->Yield(true);

   if(cli->IsChecked())
   {
      auto command = "register_account " + name_value + " " + owner_value + " " + active_value + " " +
            registrar_account_value + " " + referrer_account_value + " " + to_string(referrer_percent_value) +
            " " + broadcast_value;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
      p_GWallet->DoAssets(registrar_account_value);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->register_account(name_value, public_key_type(owner_value),
               public_key_type(active_value), registrar_account_value, referrer_account_value, referrer_percent_value, false);

         if(broadcast->IsChecked()) {
            wxRichMessageDialog confirm(this, _("Please double check and confirm operation below"),
                  _("Confirm create account?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
            confirm.ShowDetailedText(fc::json::to_pretty_string(result_obj.operations[0]));

            if (wxID_YES == confirm.ShowModal()) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->register_account(name_value, public_key_type(owner_value),
                     public_key_type(active_value), registrar_account_value, referrer_account_value, referrer_percent_value, true);
               p_GWallet->DoAssets(registrar_account_value);
            }
         }
         response = result_obj;
         new RegisterAccountResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

RegisterAccountResponse::RegisterAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Register account response"), true);
}
