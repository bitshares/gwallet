#include "include/getmyblindaccounts.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetMyBlindAccounts::GetMyBlindAccounts(GWallet* gwallet) : wxScrolledWindow() {
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *) p_GWallet);

   SetScrollRate(1, 1);

   response_grid->CreateGrid(0, 2);

   SetScrollRate(1, 1);
   response_grid->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

   gwallet->panels.p_commands->DoGridProperties(response_grid);

   response_grid->SetColLabelValue(0, _("Label"));
   response_grid->SetColLabelValue(1, _("Public key"));

   stringstream command;
   command << "get_my_blind_accounts";

   auto response =
         p_GWallet->panels.p_commands->ExecuteGetterCommand < map < string, public_key_type>>(command.str(), false, _(
         "Invalid market"));

   if (!response.IsNull()) {
      auto results = response.As<map<string, public_key_type>>();
      auto z = 0;

      response_grid->BeginBatch();
      response_grid->ClearSelection();

      response_grid->AppendRows(results.size());
      for (auto &blind : results) {

         response_grid->SetCellValue(z, 0, blind.first);
         response_grid->SetCellValue(z, 1, string(blind.second));
         ++z;
      }
      response_grid->AutoSize();
      response_grid->ForceRefresh();
      response_grid->EndBatch();
   }
}
