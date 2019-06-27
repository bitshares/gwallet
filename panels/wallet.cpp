#include "../include/panels/wallet.hpp"
#include "../include/panels/transfer.hpp"
#include "../include/panels/sellasset.hpp"
#include "../include/panels/borrowasset.hpp"
#include "../include/panels/cancelorder.hpp"
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
      DoTransfer();
   else if(selected == tree.sell_asset)
      DoSellAsset();
   else if(selected == tree.borrow_asset)
      DoBorrowAsset();
   else if(selected == tree.cancel_order)
      DoCancelOrder();
   else if(selected == tree.set_proxy)
      DoSetProxy();
   else if(selected == tree.suggest_brain_key)
      DoSuggestBrainKey();
   else if(selected == tree.get_committee_member)
      DoGetCommitteeMember();
   else if(selected == tree.get_account_history)
      DoGetAccountHistory();
   else if(selected == tree.get_order_book)
      DoGetOrderBook();
   else if(selected == tree.about)
      DoAbout();
   else if(selected == tree.info)
      DoInfo();
   else if(selected == tree.active_witnesses)
      DoActiveWitnesses();
   else if(selected == tree.active_committee)
      DoActiveCommittee();
   else if(selected == tree.upgrade_account)
      DoUpgradeAccount();
   else if(selected == tree.create_account_with_brain_key)
      DoCreateAccountWithBrainKey();
   else if(selected == tree.register_account)
      DoRegisterAccount();
   else if(selected == tree.create_asset)
      DoCreateAsset();
   else if(selected == tree.get_witness)
      DoGetWitness();
   else if(selected == tree.get_account)
      DoGetAccount();
   else if(selected == tree.get_asset)
      DoGetAsset();
   else if(selected == tree.issue_asset)
      DoIssueAsset();
   else if(selected == tree.htlc_create)
      DoCreateHtlc();
   else if(selected == tree.htlc_redeem)
      DoRedeemHtlc();
   else if(selected == tree.htlc_extend)
      DoExtendHtlc();
   else if(selected == tree.get_htlc)
      DoGetHtlc();
}

void Wallet::DoTransfer()
{
   Transfer *transfer = new Transfer(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(transfer, _("Transfer"), true);
}

void Wallet::DoSellAsset()
{
   SellAsset *sell_asset = new SellAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(sell_asset, _("Sell Asset"), true);
}

void Wallet::DoBorrowAsset()
{
   BorrowAsset *borrow_asset = new BorrowAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(borrow_asset, _("Borrow Asset"), true);
}

void Wallet::DoCancelOrder()
{
   CancelOrder *cancel_order = new CancelOrder(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(cancel_order, _("Cancel order"), true);
}

void Wallet::DoSetProxy()
{
   SetProxy *set_proxy = new SetProxy(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(set_proxy, _("Set proxy"), true);
}

void Wallet::DoSuggestBrainKey()
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::DoGetCommitteeMember()
{
   GetCommitteeMember *committee_member = new GetCommitteeMember(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(committee_member, _("Committee member"), true);
}

void Wallet::DoGetAccountHistory()
{
   GetAccountHistory *account_history = new GetAccountHistory(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(account_history, _("Account history"), true);
}

void Wallet::DoGetOrderBook()
{
   GetOrderBook *order_book = new GetOrderBook(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(order_book, _("Order book"), true);
}

void Wallet::DoAbout()
{
   About *about = new About(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(about, _("About"), true);
}

void Wallet::DoInfo()
{
   Info *info = new Info(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(info, _("Blockchain information"), true);
}

void Wallet::DoActiveWitnesses()
{
   ActiveWitnesses *active_witnesses = new ActiveWitnesses(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_witnesses, _("Active witnsses"), true);
}

void Wallet::DoActiveCommittee()
{
   ActiveCommittee *active_committee = new ActiveCommittee(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_committee, _("Active committee"), true);
}

void Wallet::DoUpgradeAccount()
{
   UpgradeAccount *upgrade_account = new UpgradeAccount(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(upgrade_account, _("Upgrade account"), true);
}

void Wallet::DoCreateAccountWithBrainKey()
{
   CreateAccountWithBrainKey *create_account_with_brain_key = new CreateAccountWithBrainKey(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(create_account_with_brain_key, _("Create account with brain key"), true);
}

void Wallet::DoRegisterAccount()
{
   RegisterAccount *register_account = new RegisterAccount(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(register_account, _("Register account"), true);
}

void Wallet::DoCreateAsset()
{
   CreateAsset *create_asset = new CreateAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(create_asset, _("Create asset"), true);
}

void Wallet::DoGetWitness()
{
   GetWitness *get_witness = new GetWitness(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(get_witness, _("Get witness"), true);
}

void Wallet::DoGetAccount()
{
   GetAccount *get_account = new GetAccount(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(get_account, _("Get account"), true);
}

void Wallet::DoGetAsset()
{
   GetAsset *get_asset = new GetAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(get_asset, _("Get asset"), true);
}

void Wallet::DoIssueAsset()
{
   IssueAsset *issue_asset = new IssueAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(issue_asset, _("Issue asset"), true);
}

void Wallet::DoCreateHtlc()
{
   CreateHtlc *create_htlc = new CreateHtlc(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(create_htlc, _("Create HTLC"), true);
}

void Wallet::DoRedeemHtlc()
{
   RedeemHtlc *redeem_htlc = new RedeemHtlc(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(redeem_htlc, _("Redeem HTLC"), true);
}

void Wallet::DoExtendHtlc()
{
   ExtendHtlc *extend_htlc = new ExtendHtlc(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(extend_htlc, _("Extend HTLC"), true);
}

void Wallet::DoGetHtlc()
{
   GetHtlc *get_htlc = new GetHtlc(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(get_htlc, _("Get HTLC"), true);
}

void Wallet::OpenCommandsPane()
{
   if(!p_GWallet->m_mgr.GetPane("Commands").IsShown()) {
      Commands *commands = new Commands(p_GWallet);
      p_GWallet->panels.p_commands = commands;
      p_GWallet->CreateCommandsPane(commands);
      p_GWallet->m_mgr.Update();

      p_GWallet->menubar->Check(XRCID("m_view_commands"), true);
   }
}
