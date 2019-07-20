#include "include/active_committee.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

ActiveCommittee::ActiveCommittee(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, updating active committee members info ..."));
   wxTheApp->Yield();

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/getters/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   response_grid->CreateGrid(0, 3);

   SetScrollRate(1,1);
   response_grid->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   p_GWallet->panels.p_commands->DoGridProperties(response_grid);

   response_grid->SetColLabelValue(0, "ID");
   response_grid->SetColLabelValue(1, _("Account"));
   response_grid->SetColLabelValue(2, _("Votes"));

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();
   auto z = 0;
   response_grid->AppendRows(info["active_committee_members"].get_array().size());
   for( auto& active_committee : info["active_committee_members"].get_array() ) {

      const auto committee = p_GWallet->bitshares.wallet_api_ptr->get_committee_member(active_committee.as_string());
      response_grid->SetCellValue(z, 0, active_committee.as_string());

      auto committee_account = p_GWallet->bitshares.wallet_api_ptr->get_account(
            string(object_id_type(committee.committee_member_account))).name;
      response_grid->SetCellValue(z, 1, committee_account);

      response_grid->SetCellValue(z, 2, wxNumberFormatter::ToString(long(committee.total_votes)));

      ++z;
   }

   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();
}
