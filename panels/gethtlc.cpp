#include "../include/panels/gethtlc.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

GetHtlc::GetHtlc(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetHtlc::OnOk));
   Connect(XRCID("htlc_id"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetHtlc::OnSearchID),
           NULL, this);
}

void GetHtlc::OnSearchID(wxCommandEvent& event)
{
}

void GetHtlc::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto htlc_id_value = htlc_id->GetValue().ToStdString();
   fc::optional<fc::variant> result_obj;
   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   try
   {
      result_obj = p_GWallet->bitshares.wallet_api_ptr->get_htlc(htlc_id_value);
      if(result_obj.valid())
         response = *result_obj;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("HTLC ID not found"));
      htlc_id->SetFocus();
      return;
   }

   new GetHtlcResponse(p_GWallet, response);

   if(cli->IsChecked())
   {
      auto command = "get_htlc " + htlc_id_value;
      p_GWallet->panels.p_cli->DoCommand(command);
   }
}

GetHtlcResponse::GetHtlcResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   SetScrollRate(1,1);
   response_tree->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   fc::variant result = any_response.As<fc::variant>();
   if(!result.is_object())
      return;

   const auto root = response_tree->AddRoot(_("HTLC object"));

   const auto transfer = response_tree->AppendItem(root, "Transfer");
   const auto from = response_tree->AppendItem(transfer, "From");
   response_tree->AppendItem(from, result["transfer"]["from"].as_string());
   const auto to = response_tree->AppendItem(transfer, "To");
   response_tree->AppendItem(to, result["transfer"]["to"].as_string());
   const auto asset = response_tree->AppendItem(transfer, "Asset");
   response_tree->AppendItem(asset, result["transfer"]["asset"].as_string());
   const auto amount = response_tree->AppendItem(transfer, "Amount");
   response_tree->AppendItem(amount, result["transfer"]["amount"].as_string());

   const auto conditions = response_tree->AppendItem(root, "Conditions");
   const auto htlc_lock = response_tree->AppendItem(conditions, "HTLC Lock");
   const auto hash_algo = response_tree->AppendItem(htlc_lock, "Hash algo");
   response_tree->AppendItem(hash_algo, result["conditions"]["htlc_lock"]["hash_algo"].as_string());
   const auto preimage_hash = response_tree->AppendItem(htlc_lock, "Preimage hash");
   response_tree->AppendItem(preimage_hash, result["conditions"]["htlc_lock"]["preimage_hash"].as_string());
   const auto preimage_size = response_tree->AppendItem(htlc_lock, "Preimage size");
   response_tree->AppendItem(preimage_size, result["conditions"]["htlc_lock"]["preimage_size"].as_string());
   const auto time_lock = response_tree->AppendItem(conditions, "Time lock");
   const auto expiration = response_tree->AppendItem(time_lock, "Expiration");
   response_tree->AppendItem(expiration, result["conditions"]["time_lock"]["expiration"].as_string());
   const auto time_left = response_tree->AppendItem(time_lock, "Time left");
   response_tree->AppendItem(time_left, result["conditions"]["time_lock"]["time_left"].as_string());

   response_tree->ExpandAll();

   gwallet->panels.p_commands->notebook->AddPage(this, _("HTLC response"), true);
}
