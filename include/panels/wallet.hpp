#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

struct treeItems
{
   wxTreeItemId transfer;
   wxTreeItemId sell_asset;
   wxTreeItemId borrow_asset;
   wxTreeItemId cancel_order;
   wxTreeItemId set_proxy;
   wxTreeItemId suggest_brain_key;
   wxTreeItemId get_committee_member;
   wxTreeItemId get_account_history;
   wxTreeItemId get_order_book;
   wxTreeItemId about;
   wxTreeItemId info;
   wxTreeItemId active_witnesses;
   wxTreeItemId active_committee;
   wxTreeItemId upgrade_account;
   wxTreeItemId create_account_with_brain_key;
   wxTreeItemId register_account;
};

class Wallet : public wxPanel
{
public:
   Wallet(GWallet* gwallet);
   void EnableOperations();
   void DisableOperations();

   GWallet* p_GWallet;

   Wallet(wxWindow *parent=NULL) {
      InitWidgetsFromXRC((wxWindow *)parent);
   }

   wxStaticText* locked_msg;

protected:
   wxTreeCtrl* wallet_tree;
   treeItems tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent) {
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Wallet"), wxT("wxPanel"));
      wallet_tree = XRCCTRL(*this,"wallet_tree",wxTreeCtrl);
      locked_msg = XRCCTRL(*this,"locked_msg",wxStaticText);
   }

   void OnCommand(wxTreeEvent& event);

   void DoTransfer();
   void DoSellAsset();
   void DoBorrowAsset();
   void DoCancelOrder();
   void DoSetProxy();
   void DoSuggestBrainKey();
   void DoGetCommitteeMember();
   void DoGetAccountHistory();
   void DoGetOrderBook();
   void DoAbout();
   void DoInfo();
   void DoActiveWitnesses();
   void DoActiveCommittee();
   void DoUpgradeAccount();
   void DoCreateAccountWithBrainKey();
   void DoRegisterAccount();

   void OpenCommandsPane();
};
