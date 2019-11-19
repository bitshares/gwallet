#include "include/redeemhtlc.hpp"

#include <panels/commands.hpp>

RedeemHtlc::RedeemHtlc(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   issuer->Append(p_GWallet->strings.accounts);
   issuer->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   preimage->SetValidator(*p_GWallet->panels.p_commands->empty_validator);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RedeemHtlc::OnOk));
   Connect(XRCID("htlc_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(RedeemHtlc::OnSearchID), NULL, this);

   SetScrollRate(1,1);
}

void RedeemHtlc::OnSearchID(wxCommandEvent& event)
{
   //p_GWallet->DoSearchID(event.GetString(), *htlc_id);
}

void RedeemHtlc::OnOk(wxCommandEvent& WXUNUSED(event))
{
   if(!Validate())
      return;

   const auto _issuer = p_GWallet->strings.accounts[issuer->GetCurrentSelection()].ToStdString();
   const auto _htlc_id = htlc_id->GetValue().ToStdString();
   const auto _preimage = preimage->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;
   auto _broadcast = true;
   if(!broadcast->IsChecked()) _broadcast = false;

   std::stringstream command;
   command << "htlc_redeem " << _htlc_id << " " << _issuer << " " << _preimage << " " << " " << std::boolalpha
           << _broadcast;
   auto response = p_GWallet->panels.p_commands->ExecuteWalletCommand(command.str(), _issuer,
         _("Confirm HTLC Redeem?"), _cli, _broadcast);

   if(!response.IsNull())
      new RedeemHtlcResponse(p_GWallet, response);
}

RedeemHtlcResponse::RedeemHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC Redeem response"), true);
}
