#include "include/registeraccount.hpp"

#include <panels/commands.hpp>

RegisterAccount::RegisterAccount(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/assets/resources"));
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
   const auto _referrer_percent = to_string(referrer_percent->GetValue());
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "register_account " << _name << " " << _owner << " " << _active << " " << _registrar_account
           << " " << _referrer_account << " " << _referrer_percent << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _registrar_account,
         _("Confirm register account?"), _cli, _broadcast);

   if(!response.IsNull())
      new RegisterAccountResponse(p_GWallet, response);
}

RegisterAccountResponse::RegisterAccountResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Register account response"), true);
}
