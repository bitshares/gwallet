#include "include/getorderbook.hpp"

#include <panels/commands.hpp>
#include <panels/cli.hpp>

GetOrderBook::GetOrderBook(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   wxXmlResource::Get()->LoadAllFiles(p_GWallet->directory + wxT("/plugins/getters/resources"));
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetOrderBook::OnOk));
   Connect(XRCID("base"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetOrderBook::OnSearchAsset), NULL, this);
   Connect(XRCID("quote"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetOrderBook::OnSearchAsset), NULL, this);
}

void GetOrderBook::OnSearchAsset(wxCommandEvent& event)
{
   p_GWallet->DoSearchAsset(event.GetString(), *wxDynamicCast(event.GetEventObject(), wxSearchCtrl));
}

void GetOrderBook::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto _base = base->GetValue().ToStdString();
   const auto _quote = quote->GetValue().ToStdString();
   const auto _limit = to_string(limit->GetValue());
   auto _cli = false;
   if(cli->IsChecked()) _cli = true;

   stringstream command;
   command << "get_order_book " << _base << " " << _quote << " " << _limit;

   auto response = p_GWallet->panels.p_commands->ExecuteGetterCommand<order_book>(command.str(), _cli,
         _("Invalid market"));

   if(!response.IsNull())
   {
      new GetOrderBookResponse(p_GWallet, response, "asks");
      new GetOrderBookResponse(p_GWallet, response, "bids");
   }
}

GetOrderBookResponse::GetOrderBookResponse(GWallet* gwallet, wxAny any_response, string response_type)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   response_grid->CreateGrid(0, 3);

   SetScrollRate(1,1);
   response_grid->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

   gwallet->panels.p_commands->DoGridProperties(response_grid);

   response_grid->SetColLabelValue(0, _("Price"));
   response_grid->SetColLabelValue(1, _("Quote"));
   response_grid->SetColLabelValue(2, _("Base"));

   auto results = any_response.As<order_book>();
   auto z = 0;

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   if(response_type == "asks") {
      response_grid->AppendRows(results.asks.size());
      for (auto &ask : results.asks) {

         response_grid->SetCellValue(z, 0, ask.price);
         response_grid->SetCellValue(z, 1, ask.quote);
         response_grid->SetCellValue(z, 2, ask.base);
         ++z;
      }
   }
   else
   {
      response_grid->AppendRows(results.bids.size());
      for (auto &bid : results.bids) {

         response_grid->SetCellValue(z, 0, bid.price);
         response_grid->SetCellValue(z, 1, bid.quote);
         response_grid->SetCellValue(z, 2, bid.base);
         ++z;
      }
   }
   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();

   gwallet->panels.p_commands->notebook->AddPage(this, _("Order book ") + response_type + _(" response"), true);
}
