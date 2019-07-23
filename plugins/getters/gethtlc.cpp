#include "include/gethtlc.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetHtlc::GetHtlc(GWallet* gwallet)
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/getters/resources"));
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
   const auto _htlc_id = htlc_id->GetValue().ToStdString();
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   stringstream command;
   command << "get_htlc " << _htlc_id;

   auto response = p_GWallet->panels.p_commands->ExecuteGetterCommand<htlc_object>(command.str(), _cli,
         _("HTLC ID not found"));

   if(!response.IsNull())
      new GetHtlcResponse(p_GWallet, response);
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
