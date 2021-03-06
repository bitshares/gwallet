#include "include/active_witnesses.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

ActiveWitnesses::ActiveWitnesses(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, updating active witnesses info ..."));
   wxTheApp->Yield();

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);
   response_grid->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   response_grid->CreateGrid(0, 4);

   gwallet->panels.p_commands->DoGridProperties(response_grid);

   response_grid->SetColLabelValue(0, "ID");
   response_grid->SetColLabelValue(1, _("Account"));
   response_grid->SetColLabelValue(2, _("Votes"));
   response_grid->SetColLabelValue(3, _("Last block"));

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();
   auto z = 0;
   response_grid->AppendRows(info["active_committee_members"].get_array().size());
   for( auto& active_witness : info["active_witnesses"].get_array() ) {

      const auto witness = p_GWallet->bitshares.wallet_api_ptr->get_witness(active_witness.as_string());
      response_grid->SetCellValue(z, 0, active_witness.as_string());

      auto witness_account = p_GWallet->bitshares.wallet_api_ptr->get_account(
            string(object_id_type(witness.witness_account))).name;
      response_grid->SetCellValue(z, 1, witness_account);

      response_grid->SetCellValue(z, 2, wxNumberFormatter::ToString(long(witness.total_votes)));
      response_grid->SetCellValue(z, 3, wxNumberFormatter::ToString(long(witness.last_confirmed_block_num)));

      ++z;
   }

   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();
}
