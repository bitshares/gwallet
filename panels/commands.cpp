#include "../include/panels/commands.hpp"
#include "../include/panels/wallet.hpp"

Commands::Commands(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);
}

void Commands::DoSignedTranactionResponse(wxTreeCtrl* response_tree, signed_transaction result)
{
   response_tree->ShowScrollbars(wxSHOW_SB_DEFAULT,wxSHOW_SB_NEVER);

   const auto root = response_tree->AddRoot(_("Signed Transaction"));

   const auto ref_block_num = response_tree->AppendItem(root, _("Reference block number"));
   response_tree->AppendItem(ref_block_num, to_string(result.ref_block_num));

   const auto ref_block_prefix = response_tree->AppendItem(root, _("Reference block prefix"));
   response_tree->AppendItem(ref_block_prefix, to_string(result.ref_block_prefix));

   const auto expiration = response_tree->AppendItem(root, _("Expiration"));
   response_tree->AppendItem(expiration, result.expiration.to_iso_string());

   const auto operations = response_tree->AppendItem(root, _("Operations"));
   response_tree->AppendItem(operations, fc::json::to_string(result.operations));

   const auto extensions = response_tree->AppendItem(root, _("Extensions"));
   response_tree->AppendItem(extensions, fc::json::to_string(result.extensions));

   const auto signatures = response_tree->AppendItem(root, _("Signatures"));
   response_tree->AppendItem(signatures, fc::json::to_string(result.signatures));

   response_tree->ExpandAll();
}

void Commands::Wait()
{
   wxBusyCursor wait;
   wxTheApp->Yield(true);
}

void Commands::DoGridProperties(wxGrid* grid)
{
   grid->EnableDragCell();
   grid->EnableDragColMove();
   grid->EnableDragColSize();
   grid->EnableDragGridSize();
   grid->EnableDragRowSize();
}

uint32_t Commands::DoDateToSeconds(wxDatePickerCtrl* date, wxTimePickerCtrl* time)
{
   const auto date_value = date->GetValue().ToUTC().GetValue().ToLong()/1000;
   const auto time_second = time->GetValue().ToUTC().GetSecond();
   const auto time_minute = time->GetValue().ToUTC().GetMinute();
   const auto time_hour = time->GetValue().ToUTC().GetHour();
   const auto time_value = (time_hour*3600) + (time_minute*60) + time_second;

   auto now = wxDateTime::Now().ToUTC().GetValue().ToLong()/1000;
   uint32_t seconds = date_value + time_value - now;
   return seconds;
}