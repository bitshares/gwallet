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

#include "../include/panels/commands.hpp"

Wallet::Wallet(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   const auto root = wallet_tree->AddRoot(_("Operations"));

   tree.transfer = wallet_tree->AppendItem(root, _("Transfer"));
   tree.sell_asset = wallet_tree->AppendItem(root, _("Sell asset"));
   tree.borrow_asset = wallet_tree->AppendItem(root, _("Borrow asset"));
   tree.cancel_order = wallet_tree->AppendItem(root, _("Cancel order"));
   tree.set_proxy = wallet_tree->AppendItem(root, _("Set proxy"));
   tree.suggest_brain_key = wallet_tree->AppendItem(root, _("Suggest brain key"));
   tree.get_committee_member = wallet_tree->AppendItem(root, _("Get committee member"));
   tree.get_account_history = wallet_tree->AppendItem(root, _("Get account history"));
   tree.get_order_book = wallet_tree->AppendItem(root, _("Get order book"));
   tree.about = wallet_tree->AppendItem(root, _("About"));
   tree.info = wallet_tree->AppendItem(root, _("Info"));
   tree.active_witnesses = wallet_tree->AppendItem(root, _("Active witnesses"));
   tree.active_committee = wallet_tree->AppendItem(root, _("Active committee"));
   tree.upgrade_account = wallet_tree->AppendItem(root, _("Upgrade account"));
   tree.create_account_with_brain_key = wallet_tree->AppendItem(root, _("Create account with brain key"));
   tree.register_account = wallet_tree->AppendItem(root, _("Register account"));

   wallet_tree->ExpandAll();

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
}

void Wallet::DoTransfer()
{
   Transfer *transfer = new Transfer(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(transfer, _("Transfer"));
}

void Wallet::DoSellAsset()
{
   SellAsset *sell_asset = new SellAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(sell_asset, _("Sell Asset"));
}

void Wallet::DoBorrowAsset()
{
   BorrowAsset *borrow_asset = new BorrowAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(borrow_asset, _("Borrow Asset"));
}

void Wallet::DoCancelOrder()
{
   CancelOrder *cancel_order = new CancelOrder(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(cancel_order, _("Cancel order"));
}

void Wallet::DoSetProxy()
{
   SetProxy *set_proxy = new SetProxy(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(set_proxy, _("Set proxy"));
}

void Wallet::DoSuggestBrainKey()
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::DoGetCommitteeMember()
{
   GetCommitteeMember *committee_member = new GetCommitteeMember(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(committee_member, _("Committee member"));
}

void Wallet::DoGetAccountHistory()
{
   GetAccountHistory *account_history = new GetAccountHistory(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(account_history, _("Account history"));
}

void Wallet::DoGetOrderBook()
{
   GetOrderBook *order_book = new GetOrderBook(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(order_book, _("Order book"));
}

void Wallet::DoAbout()
{
   About *about = new About(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(about, _("About"));
}

void Wallet::DoInfo()
{
   Info *info = new Info(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(info, _("Blockchain information"));
}

void Wallet::DoActiveWitnesses()
{
   ActiveWitnesses *active_witnesses = new ActiveWitnesses(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_witnesses, _("Active witnsses"));
}

void Wallet::DoActiveCommittee()
{
   ActiveCommittee *active_committee = new ActiveCommittee(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_committee, _("Active committee"));
}

void Wallet::DoUpgradeAccount()
{
   UpgradeAccount *upgrade_account = new UpgradeAccount(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(upgrade_account, _("Upgrade account"));
}

void Wallet::DoCreateAccountWithBrainKey()
{
   CreateAccountWithBrainKey *create_account_with_brain_key = new CreateAccountWithBrainKey(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(create_account_with_brain_key, _("Create account with brain key"));
}

void Wallet::DoRegisterAccount()
{
   RegisterAccount *register_account = new RegisterAccount(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(register_account, _("Register account"));
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
