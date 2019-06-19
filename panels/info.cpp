#include "../include/panels/info.hpp"
#include "../include/panels/active_committee.hpp"
#include "../include/panels/active_witnesses.hpp"

#include "../include/panels/commands.hpp"

Info::Info(GWallet* gwallet) : wxScrolledWindow()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   head_block_num->SetLabel(wxNumberFormatter::ToString((long)info["head_block_num"].as_uint64()));
   head_block_age->SetLabel(info["head_block_age"].as_string());
   next_maintenance_time->SetLabel(info["next_maintenance_time"].as_string());
   participation->SetLabel(info["participation"].as_string());
   chain_id->SetLabel(info["chain_id"].as_string());
   core_asset->SetLabel(GRAPHENE_SYMBOL);
   blockchain_name->SetLabel("Bitshares 3.0");

   Connect(XRCID("view_witnesses"), wxEVT_HYPERLINK, wxHyperlinkEventHandler(Info::OnWitness), NULL, this);
   Connect(XRCID("view_committee"), wxEVT_HYPERLINK, wxHyperlinkEventHandler(Info::OnCommittee), NULL, this);
}

void Info::OnWitness(wxHyperlinkEvent& event)
{
   ActiveWitnesses *active_witnesses = new ActiveWitnesses(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_witnesses, _("Active witnesses"));
}

void Info::OnCommittee(wxHyperlinkEvent& event)
{
   ActiveCommittee *active_committee = new ActiveCommittee(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_committee, _("Active committee"));
}
