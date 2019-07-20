#include "include/extendhtlc.hpp"

#include <panels/commands.hpp>

ExtendHtlc::ExtendHtlc(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/htlc/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   issuer->Append(p_GWallet->strings.accounts);
   issuer->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExtendHtlc::OnOk));
   Connect(XRCID("htlc_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(ExtendHtlc::OnSearchID), NULL, this);

   SetScrollRate(1,1);
}

void ExtendHtlc::OnSearchID(wxCommandEvent& event)
{
   //p_GWallet->DoSearchID(event.GetString(), *htlc_id);
}

void ExtendHtlc::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _issuer = p_GWallet->strings.accounts[issuer->GetCurrentSelection()].ToStdString();
   const auto _htlc_id = htlc_id->GetValue().ToStdString();
   const auto _seconds_to_add = to_string(minutes_to_add->GetValue() * 60);
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   stringstream command;
   command << "htlc_extend " << _htlc_id << " " << _issuer << " " << _seconds_to_add << " " << std::boolalpha << _broadcast;

   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _issuer,
         _("Confirm HTLC Extend?"), _cli, _broadcast);

   if(!response.IsNull())
      new ExtendHtlcResponse(p_GWallet, response);
}

ExtendHtlcResponse::ExtendHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC Extend response"), true);
}
