#include "../include/panels/wallet.hpp"
#include "../plugins/transfer/include/transfer.hpp"
#include "../plugins/sellasset/include/sellasset.hpp"
#include "../plugins/borrowasset/include/borrowasset.hpp"
#include "../plugins/cancelorder/include/cancelorder.hpp"
#include "../include/panels/setproxy.hpp"
#include "../include/panels/getcommitteemember.hpp"
#include "../include/panels/getaccounthistory.hpp"
#include "../include/panels/getorderbook.hpp"
#include "../include/panels/about.hpp"
#include "../include/panels/info.hpp"
#include "../include/panels/active_committee.hpp"
#include "../include/panels/active_witnesses.hpp"
#include "../include/panels/upgradeaccount.hpp"
#include "../include/panels/createaccountwithbrainkey.hpp"
#include "../include/panels/registeraccount.hpp"
#include "../include/panels/createasset.hpp"
#include "../include/panels/getwitness.hpp"
#include "../include/panels/getaccount.hpp"
#include "../include/panels/getasset.hpp"
#include "../include/panels/issueasset.hpp"
#include "../include/panels/createhtlc.hpp"
#include "../include/panels/redeemhtlc.hpp"
#include "../include/panels/extendhtlc.hpp"
#include "../include/panels/gethtlc.hpp"
#include "../include/panels/updatefeedproducers.hpp"
#include "../include/panels/voteforwitness.hpp"

#include "../include/panels/commands.hpp"

Wallet::Wallet(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   const auto root = wallet_tree->AddRoot(_("Operations"));

   const auto asset = wallet_tree->AppendItem(root, _("Asset"));
   tree.create_asset = wallet_tree->AppendItem(asset, _("Create"));
   tree.issue_asset = wallet_tree->AppendItem(asset, _("Issue"));
   tree.transfer = wallet_tree->AppendItem(asset, _("Transfer"));
   tree.sell_asset = wallet_tree->AppendItem(asset, _("Sell"));
   tree.borrow_asset = wallet_tree->AppendItem(asset, _("Borrow"));
   tree.cancel_order = wallet_tree->AppendItem(asset, _("Cancel order"));
   tree.update_feed_producers = wallet_tree->AppendItem(asset, _("Update feed producers"));
   wallet_tree->Expand(asset);

   const auto account = wallet_tree->AppendItem(root, _("Account"));
   tree.register_account = wallet_tree->AppendItem(account, _("Register"));
   tree.create_account_with_brain_key = wallet_tree->AppendItem(account, _("Create with brain key"));
   tree.upgrade_account = wallet_tree->AppendItem(account, _("Upgrade"));
   tree.set_proxy = wallet_tree->AppendItem(account, _("Set proxy"));
   tree.suggest_brain_key = wallet_tree->AppendItem(account, _("Suggest brain key"));

   const auto getter = wallet_tree->AppendItem(root, _("Getters"));
   tree.about = wallet_tree->AppendItem(getter, _("About"));
   tree.info = wallet_tree->AppendItem(getter, _("Info"));
   tree.get_committee_member = wallet_tree->AppendItem(getter, _("Committee member"));
   tree.get_witness = wallet_tree->AppendItem(getter, _("Witness"));
   tree.get_account = wallet_tree->AppendItem(getter, _("Account"));
   tree.get_account_history = wallet_tree->AppendItem(getter, _("Account history"));
   tree.get_asset = wallet_tree->AppendItem(getter, _("Asset"));
   tree.get_order_book = wallet_tree->AppendItem(getter, _("Order book"));
   tree.active_witnesses = wallet_tree->AppendItem(getter, _("Active witnesses"));
   tree.active_committee = wallet_tree->AppendItem(getter, _("Active committee"));
   tree.get_htlc = wallet_tree->AppendItem(getter, _("HTLC"));

   const auto htlc = wallet_tree->AppendItem(root, _("HTLC"));
   tree.htlc_create = wallet_tree->AppendItem(htlc, _("Create"));
   tree.htlc_redeem = wallet_tree->AppendItem(htlc, _("Redeem"));
   tree.htlc_extend = wallet_tree->AppendItem(htlc, _("Extend"));

   const auto governance = wallet_tree->AppendItem(root, _("Governance"));
   tree.vote_for_witness = wallet_tree->AppendItem(governance, _("Vote for witness"));

   wallet_tree->Expand(root);

   if(p_GWallet->bitshares.wallet_api_ptr->is_locked())
      DisableOperations();
   else
      EnableOperations();

   Connect(wallet_tree->GetId(), wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(Wallet::OnCommand), NULL, this);
}

void Wallet::EnableOperations()
{
   wallet_tree->Enable(true);
   locked_msg->SetForegroundColour(wxNullColour);
   locked_msg->SetLabel(_("Please double click a command from below"));
}

void Wallet::DisableOperations()
{
   wallet_tree->Enable(false);
}

void Wallet::OnCommand(wxTreeEvent& event)
{
   auto selected = wallet_tree->GetSelection();

   OpenCommandsPane();

   if(selected == tree.transfer)
      DoOperation<Transfer>(_("Transfer"));
   else if(selected == tree.sell_asset)
      DoOperation<SellAsset>(_("Sell asset"));
   else if(selected == tree.borrow_asset)
      DoOperation<BorrowAsset>(_("Borrow asset"));
   else if(selected == tree.cancel_order)
      DoOperation<CancelOrder>(_("Cancel order"));
   else if(selected == tree.set_proxy)
      DoOperation<SetProxy>(_("Set proxy"));
   else if(selected == tree.suggest_brain_key)
      DoSuggestBrainKey();
   else if(selected == tree.get_committee_member)
      DoOperation<GetCommitteeMember>(_("Get committee member"));
   else if(selected == tree.get_account_history)
      DoOperation<GetAccountHistory>(_("Get account history"));
   else if(selected == tree.get_order_book)
      DoOperation<GetOrderBook>(_("Get order book"));
   else if(selected == tree.about)
      DoOperation<About>(_("About"));
   else if(selected == tree.info)
      DoOperation<Info>(_("Info"));
   else if(selected == tree.active_witnesses)
      DoOperation<ActiveWitnesses>(_("Get active witnesses"));
   else if(selected == tree.active_committee)
      DoOperation<ActiveCommittee>(_("Get active committee"));
   else if(selected == tree.upgrade_account)
      DoOperation<UpgradeAccount>(_("Upgrade account"));
   else if(selected == tree.create_account_with_brain_key)
      DoOperation<CreateAccountWithBrainKey>(_("Create account with brain key"));
   else if(selected == tree.register_account)
      DoOperation<RegisterAccount>(_("Register account"));
   else if(selected == tree.create_asset)
      DoOperation<CreateAsset>(_("Create asset"));
   else if(selected == tree.get_witness)
      DoOperation<GetWitness>(_("Get witness"));
   else if(selected == tree.get_account)
      DoOperation<GetAccount>(_("Get account"));
   else if(selected == tree.get_asset)
      DoOperation<GetAsset>(_("Get asset"));
   else if(selected == tree.issue_asset)
      DoOperation<IssueAsset>(_("Issue asset"));
   else if(selected == tree.htlc_create)
      DoOperation<CreateHtlc>(_("Create HTLC"));
   else if(selected == tree.htlc_redeem)
      DoOperation<RedeemHtlc>(_("Redeem HTLC"));
   else if(selected == tree.htlc_extend)
      DoOperation<ExtendHtlc>(_("Extend HTLC"));
   else if(selected == tree.get_htlc)
      DoOperation<GetHtlc>(_("Get HTLC"));
   else if(selected == tree.update_feed_producers)
      DoOperation<UpdateAssetFeedProducers>(_("Update feed producers"));
   else if(selected == tree.vote_for_witness)
      DoOperation<VoteForWitness>(_("Vote for witness"));

   p_GWallet->m_mgr.GetPane("Commands").window->SetFocus();
}

void Wallet::DoSuggestBrainKey()
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

template<typename T>
void Wallet::DoOperation(wxString title)
{
   T *operation = new T(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(operation, title, true);
}

void Wallet::OpenCommandsPane()
{
   if(!p_GWallet->m_mgr.GetPane("Commands").IsShown()) {
      Commands *commands = new Commands(p_GWallet);
      p_GWallet->panels.p_commands = commands;
      p_GWallet->CreatePane(commands, "Commands", this->GetClientSize().x - this->GetClientSize().x/4,
            this->GetClientSize().y - this->GetClientSize().y/4, 1, false);
      p_GWallet->m_mgr.Update();

      p_GWallet->menubar->Check(XRCID("m_view_commands"), true);
   }
}
