#pragma once

#include "bitshares.hpp"

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <wx/config.h>
#include <wx/grid.h>
#include <wx/xrc/xmlres.h>
#include <wx/aui/aui.h>

#include <wx/busyinfo.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/hyperlink.h>
#include <wx/filedlg.h>
#include <wx/filectrl.h>
#include <wx/xrc/xh_auinotbk.h>
#include <wx/srchctrl.h>

class Info;
class Cli;
class Wallet;

class Commands;

struct States {
   bool is_noconfig = false;
   bool is_connected = false;
   bool is_new = false;
   bool is_locked = false;
   bool is_unlocked = false;
   bool is_account_linked = false;
   bool modes_created = false;
};

struct Panels {
   Cli* p_cli;
   Wallet* p_wallet;
   Commands* p_commands;
};

struct Strings {
   wxArrayString accounts;
   wxArrayString assets;
   wxArrayString balances;
   wxArrayString precisions;

   wxString first_account_name;
   wxString selected_account;
   wxString selected_asset;
};

class GWallet : public wxFrame
{
public:
   wxConfig* config;
   wxString directory;
   States state;
   Bitshares bitshares;
   Panels panels;
   Strings strings;
   wxAuiManager m_mgr;
   wxMenuBar* menubar;

   GWallet(const wxString& title);
   ~GWallet() { m_mgr.UnInit(); }

   void DoState();
   void OnError(wxWindow* parent, wxString msg);
   void DoAssets(std::string account);
   void DoAccounts();
   void DoModes();
   std::string DoPrettyBalance(int precision, double balance);
   void CreateCommandsPane(Commands* commands);
   void DoSearchAccount(const wxString& keyword, wxSearchCtrl& account_field);
   void DoSearchAsset(const wxString& keyword, wxSearchCtrl& asset_field);

protected:
   wxToolBar* toolbar;
   wxComboBox* t_accounts;
   wxComboBox* t_assets;
   wxStaticText* t_balance;
   wxPanel* main_panel;
   wxBitmapButton* connect_button;

private:
   wxLocale* locale;

   void InitWidgetsFromXRC(wxWindow *parent) {
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GWallet"), wxT("wxFrame"));
      menubar = XRCCTRL(*this,"menubar",wxMenuBar);
      toolbar = XRCCTRL(*this,"toolbar",wxToolBar);
      t_accounts = XRCCTRL(*this,"t_accounts",wxComboBox);
      t_assets = XRCCTRL(*this,"t_assets",wxComboBox);
      t_balance = XRCCTRL(*this,"t_balance",wxStaticText);
      main_panel = XRCCTRL(*this,"main_panel",wxPanel);
      connect_button = XRCCTRL(*this,"connect_button",wxBitmapButton);
   }

   void OnNew(wxCommandEvent& event);
   void OnOpen(wxCommandEvent& event);
   void OnSave(wxCommandEvent& event);
   void OnNetwork(wxCommandEvent& event);
   void OnQuit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);
   void OnChangeLanguage(wxCommandEvent& event);
   void OnConnect(wxCommandEvent& event);
   void OnDisconnect(wxCommandEvent& event);
   void OnSetPassword(wxCommandEvent& event);
   void OnLock(wxCommandEvent& event);
   void OnUnlock(wxCommandEvent& event);
   void OnImportKey(wxCommandEvent& event);
   void OnImportBalance(wxCommandEvent& event);
   void OnChangeAccount(wxCommandEvent& event);
   void OnChangeAsset(wxCommandEvent& event);
   void CreateEvents();
   void DoInitialConfig();
   void DoInitialSize();
   void LoadRegistrationWizardWidget();
   void SelectLanguage(int lang);
   void OnViewWelcome(wxCommandEvent& event);
   void OnViewCommands(wxCommandEvent& event);
   void OnViewWallet(wxCommandEvent& event);
   void OnViewCli(wxCommandEvent& event);
   void CreateWalletPane(Wallet* wallet);
   void CreateCliPane(Cli* cli);
   void OnPanelClose(wxAuiManagerEvent& event);
};
