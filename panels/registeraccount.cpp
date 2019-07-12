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

   name->SetValidator(*p_GWallet->panels.p_commands->empty_validator);
   owner->SetValidator(*p_GWallet->panels.p_commands->empty_validator);
   active->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RegisterAccount::OnOk));
   Connect(XRCID("referrer_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(RegisterAccount::OnSearchAccount), NULL, this);
}

void RegisterAccount::OnSearchAccount(wxCommandEvent& event)
{
   p_GWallet->DoSearchAccount(event.GetString(), *referrer_account);
}

void RegisterAccount::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _owner = owner->GetValue().ToStdString();
   const auto _active = active->GetValue().ToStdString();
   const auto _name = name->GetValue().ToStdString();
   const auto _registrar_account = registrar_account->GetValue().ToStdString();
   const auto _referrer_account = referrer_account->GetValue().ToStdString();
   const auto _referrer_percent = referrer_percent->GetValue();

   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   signed_transaction result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      auto command = "register_account " + _name + " " + _owner + " " + _active + " " +
            _registrar_account + " " + _referrer_account + " " + to_string(_referrer_percent) +
            " " + _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command);
      p_GWallet->DoAssets(_registrar_account);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->register_account(_name, public_key_type(_owner),
               public_key_type(_active), _registrar_account, _referrer_account, _referrer_percent, false);

         if(broadcast->IsChecked()) {
            wxRichMessageDialog confirm(this, _("Please double check and confirm operation below"),
                  _("Confirm create account?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
            confirm.ShowDetailedText(fc::json::to_pretty_string(result_obj.operations[0]));

            if (wxID_YES == confirm.ShowModal()) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->register_account(_name, public_key_type(_owner),
                     public_key_type(_active), _registrar_account, _referrer_account, _referrer_percent, true);
               p_GWallet->DoAssets(_registrar_account);
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
