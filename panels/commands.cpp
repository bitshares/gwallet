#include "../include/panels/commands.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"

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

optional<wxAny> Commands::ValidateAccount(wxSearchCtrl* control)
{
   try
   {
      return p_GWallet->bitshares.wallet_api_ptr->get_account(control->GetValue().ToStdString());
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is invalid"));
      control->SetFocus();
      return {};
   }
}

optional<wxAny> Commands::ValidateAsset(wxSearchCtrl* control)
{
   try
   {
      return p_GWallet->bitshares.wallet_api_ptr->get_asset(control->GetValue().ToStdString());
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Asset is invalid"));
      control->SetFocus();
      return {};
   }
}

wxAny Commands::ExecuteWalletCommand(string command_string, string account, wxString confirm, bool cli, bool broadcast)
{
   wxAny response;
   p_GWallet->panels.p_commands->Wait();

   const auto wallet_api = fc::api<graphene::wallet::wallet_api>(p_GWallet->bitshares.wallet_api_ptr);
   const auto api_id = p_GWallet->bitshares.wallet_cli->register_api(wallet_api);
   const fc::variants line_variants = fc::json::variants_from_string(command_string);
   const auto command_name = line_variants[0].get_string();
   auto arguments_variants = fc::variants( line_variants.begin()+1,line_variants.end());

   if(cli)
   {
      p_GWallet->panels.p_cli->DoCommand(command_string);
      p_GWallet->DoAssets(account);
      return {};
   }
   else
   {
      // broadcast is always false in the first one
      arguments_variants[arguments_variants.size()-1] = false;
      try {

         auto result_obj = p_GWallet->bitshares.wallet_cli->receive_call(api_id, command_name, arguments_variants);
         auto st = result_obj.as<signed_transaction>(GRAPHENE_MAX_NESTED_OBJECTS);
         response = st;

         if(broadcast) {
            wxRichMessageDialog _confirm(this, _("Please double check and confirm operation below"),
                  confirm, wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION);
            _confirm.ShowDetailedText(fc::json::to_pretty_string(st.operations[0]));
            if (wxID_YES == _confirm.ShowModal()) {
               wxTheApp->Yield(true);
               arguments_variants[arguments_variants.size()-1] = true;
               result_obj = p_GWallet->bitshares.wallet_cli->receive_call(api_id, command_name, arguments_variants);
               p_GWallet->DoAssets(account);
            }
            else {
               return {};
            }
            st = result_obj.as<signed_transaction>(GRAPHENE_MAX_NESTED_OBJECTS);
            response = st;
         }
         return response;
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
         return {};
      }
   }
}

template<typename T>
wxAny Commands::ExecuteGetterCommand(string command_string, bool cli, wxString error_message)
{
   wxAny response;
   p_GWallet->panels.p_commands->Wait();

   const auto wallet_api = fc::api<graphene::wallet::wallet_api>(p_GWallet->bitshares.wallet_api_ptr);
   const auto api_id = p_GWallet->bitshares.wallet_cli->register_api(wallet_api);
   const fc::variants line_variants = fc::json::variants_from_string(command_string);
   const auto command_name = line_variants[0].get_string();
   auto arguments_variants = fc::variants( line_variants.begin()+1,line_variants.end());

   if(cli)
   {
      p_GWallet->panels.p_cli->DoCommand(command_string);
      return {};
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_cli->receive_call(api_id, command_name, arguments_variants);
         auto casted = result_obj.as<T>(GRAPHENE_MAX_NESTED_OBJECTS);
         response = casted;
         return response;
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, error_message);
         return {};
      }
   }
}

// needed for linker purposes
template wxAny Commands::ExecuteGetterCommand<asset_object>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<account_object>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<witness_object>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<committee_member_object>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<order_book>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<htlc_object>(string command_string, bool cli, wxString error_message);
template wxAny Commands::ExecuteGetterCommand<vector<graphene::wallet::operation_detail>>(string command_string, bool cli,
      wxString error_message);
