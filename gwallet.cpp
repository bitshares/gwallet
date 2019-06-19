#ifndef GWALLET_H
#define GWALLET_H
#include "include/gwallet.hpp"
#endif
#include "include/dialogs/importkey.hpp"
#include "include/dialogs/importbalance.hpp"

#include "include/wizards/registration.hpp"

#include "include/panels/info.hpp"
#include "include/panels/about.hpp"

#include "include/panels/cli.hpp"
#include "include/panels/wallet.hpp"
#include "include/panels/dashboard.hpp"
#include "include/panels/commands.hpp"

GWallet::GWallet(const wxString& title)
{
   DoInitialConfig();
   DoInitialSize();

   CreateEvents();
   DoState();

   // todo: delete all config, enable to start with an empty config, could be command line option
   //config->DeleteAll();

   //m_mgr.SetManagedWindow(this);

   Centre();
}

void GWallet::OnNew(wxCommandEvent& WXUNUSED(event))
{
   LoadRegistrationWizardWidget();
}

void GWallet::OnOpen(wxCommandEvent& WXUNUSED(event))
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString defaultDir(f.GetPath());

   wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard);
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString path = dialog.GetPath();
      config->Write("WalletPath", path);

      // get ws_server form wallet
      auto _wallet = fc::json::from_file( path.ToStdString() ).as< graphene::wallet::wallet_data >( 2 * GRAPHENE_MAX_NESTED_OBJECTS );
      wxString ws_server(_wallet.ws_server);
      config->Write("Server", ws_server);

      config->Flush();

      if(state.is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, XRCID("t_disconnect"));
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }

      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("t_connect"));
      ProcessWindowEvent(event_connect);
   }
}

void GWallet::OnSave(wxCommandEvent& WXUNUSED(event))
{
   const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   const wxString defaultDir(f.GetPath());

   const wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard, wxFC_SAVE);
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString path = dialog.GetPath();
      bitshares.wallet_api_ptr->save_wallet_file(path.ToStdString());
      config->Write("WalletPath", path);
      config->Flush();
   }
}

void GWallet::OnNetwork(wxCommandEvent& WXUNUSED(event))
{
   wxTextEntryDialog dialog(this, _("Enter server"), _("Websocket endpoint"));
   if ( dialog.ShowModal() == wxID_OK )
   {
      const wxString ws_server = dialog.GetValue();
      config->Write("Server", ws_server);
      config->Flush();

      if(state.is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_DISCONNECT"));
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_CONNECT"));
      ProcessWindowEvent(event_connect);
   }
}

void GWallet::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(true);
}

void GWallet::OnConnect(wxCommandEvent& WXUNUSED(event))
{
   wxString path;
   wxString server;
   if (config->Read("WalletPath", &path) && config->Read("Server", &server)) {
      try {
         wxWindowDisabler disableAll;
         wxBusyInfo wait(_("Please wait, connecting ..."));
         wxTheApp->Yield();
         bitshares.Connect(server.ToStdString(), path.ToStdString());
      }
      catch(const fc::exception &e) {
         OnError(this, _("Problem connecting to:") + "\n\n" + server + "\n\n" + _("Please try again or change server."));
         return;
      }

      if (bitshares.wallet_api_ptr->is_new()) {
         state.is_new = true;
         state.is_locked = false;
         state.is_unlocked = false;
         state.is_account_linked = false;
      } else {
         if (bitshares.wallet_api_ptr->is_locked()) {
            state.is_new = false;
            state.is_locked = true;
            state.is_unlocked = false;
            state.is_account_linked = true;
         } else {
            state.is_new = false;
            state.is_locked = false;
            state.is_unlocked = true;
            state.is_account_linked = true;
         }

         DoAccounts();
         DoAssets(strings.first_account_name.ToStdString());

         strings.selected_account = strings.first_account_name;
         strings.selected_asset = strings.assets[0];

         if(!state.modes_created) {
            DoModes();
            state.modes_created = true;
         }
      }
      state.is_connected = true;
      DoState();
   }
}

void GWallet::OnDisconnect(wxCommandEvent& WXUNUSED(event))
{
   try {
      bitshares.Disconnect();
   }
   catch(const fc::exception& e)
   {
      OnError(this, _("Some problem when disconnecting, try again ..."));
   }
   SetStatusText(_("Disconnected"));

   state.is_connected = false;
   DoState();
}

void GWallet::OnSetPassword(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Enter password"));
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString password = dialog.GetValue();

      bitshares.wallet_api_ptr->set_password(password.ToStdString());
      SetStatusText(_("Connected | Locked"));

      state.is_new = false;
      state.is_locked = true;
      DoState();

      if(!state.modes_created) {
         DoModes();
         state.modes_created = true;
      }
   }
}
void GWallet::OnAbout(wxCommandEvent& WXUNUSED(event))
{
   wxString msg;
   msg.Printf(_("Secure and simple bitshares wallet build with %s"), wxVERSION_STRING);
   wxMessageBox(msg, _("About G-Wallet"), wxOK | wxICON_INFORMATION, this);
}

void GWallet::OnLock(wxCommandEvent & WXUNUSED(event))
{
   try
   {
      bitshares.wallet_api_ptr->lock();
   }
   catch(const fc::exception& e)
   {
      OnError(this, _("Some problem when locking, try again ..."));
      return;
   }
   state.is_locked = true;
   state.is_unlocked = false;
   DoState();

   panels.p_wallet->DisableOperations();
   panels.p_wallet->locked_msg->SetForegroundColour("Red");
   panels.p_wallet->locked_msg->SetLabel(_("Please double click a command from below"));
}

void GWallet::OnUnlock(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Unlock wallet"));
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString password = dialog.GetValue();

      try {
         bitshares.wallet_api_ptr->unlock(password.ToStdString());
      }
      catch(const fc::exception& e)
      {
         OnError(this, _("Password is incorrect, please try again."));
         return;
      }
      state.is_locked = false;
      state.is_unlocked = true;
      DoState();

      panels.p_wallet->EnableOperations();
   }
}
void GWallet::OnImportKey(wxCommandEvent& WXUNUSED(event))
{
   ImportKeyDialog importKeyDialog( this );
}

void GWallet::OnImportBalance(wxCommandEvent& WXUNUSED(event))
{
   ImportBalanceDialog importBalanceDialog( this );
}

void GWallet::OnChangeAccount(wxCommandEvent& WXUNUSED(event))
{
   const auto selected = t_accounts->GetCurrentSelection();
   const auto account_name = strings.accounts[selected];

   DoAssets(account_name.ToStdString());

   strings.selected_account = strings.accounts[selected];

   if(strings.assets.size() > 0)
      strings.selected_asset = strings.assets[0];
}

void GWallet::OnChangeAsset(wxCommandEvent& WXUNUSED(event))
{
   const auto selected = t_assets->GetCurrentSelection();

   const auto asset_name = strings.assets[selected];
   const auto balance = strings.balances[selected].ToStdString();
   const auto precision = strings.precisions[selected].ToStdString();

   auto pretty_balance = DoPrettyBalance(std::stoi(precision), std::stod(balance));
   t_balance->SetLabel(pretty_balance + " " + asset_name);
   strings.selected_asset = asset_name;
}

void GWallet::DoInitialSize()
{
   wxImage::AddHandler(new wxPNGHandler);

   InitWidgetsFromXRC((wxWindow *)NULL);

   wxTopLevelWindow::Maximize(true);
   //wxTopLevelWindow::SetMinSize(wxSize(600, 450));
}

void GWallet::DoInitialConfig()
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   directory = f.GetPath();

   wxXmlResource::Get()->InitAllHandlers();
   wxXmlResource::Get()->LoadAllFiles(directory + wxT("/resources"));

   const wxIcon application_icon(directory + wxT("/icons/btslogo.png"), wxBITMAP_TYPE_PNG);
   SetIcon(application_icon);

   config = new wxConfig(wxT("GWallet"));

   wxString path;
   wxString server;
   bool allset;
   if (!config->Read("WalletPath", &path) || !config->Read("Server", &server)) {
      state.is_noconfig = true;
   } else {
      state.is_noconfig = false;
      if(config->Read("AllSet", &allset))
         state.is_account_linked = true;
   }

   wxString i18n_dir(directory + wxFILE_SEP_PATH + wxT("i18n"));
   int lang;
   if (config->Read("Lang", &lang)) {
      locale = new wxLocale(lang);
      locale->Init(lang);
   }
   else {
      locale = new wxLocale(wxLANGUAGE_DEFAULT);
      locale->Init(wxLANGUAGE_DEFAULT);
   }
   locale->AddCatalogLookupPathPrefix(i18n_dir);
   locale->AddCatalog(wxT("gwallet"));
}

void GWallet::DoAssets(std::string account)
{
   t_assets->Clear();
   strings.assets.Clear();

   int n = 0;

   const auto& account_balances = bitshares.wallet_api_ptr->list_account_balances(account);
   for(auto& account_balance : account_balances) {

      std::string asset_id_string = std::string(object_id_type(account_balance.asset_id));

      const auto& asset_object = bitshares.wallet_api_ptr->get_asset(asset_id_string);
      const auto& asset_symbol = asset_object.symbol;
      const auto& precision = asset_object.precision;
      const auto& amount_value = account_balance.amount.value;

      if(n == 0) {
         auto pretty_balance = DoPrettyBalance(precision, amount_value);
         t_balance->SetLabel(pretty_balance + " " + asset_symbol);
      }
      strings.assets.Add(asset_symbol);
      strings.balances.Add(fc::to_string(amount_value));
      strings.precisions.Add(fc::to_string(precision));
      n++;
   }
   t_assets->Set(strings.assets);
   t_assets->SetSelection(0);
   t_assets->Enable(true);
}

void GWallet::DoAccounts()
{
   const auto& accounts = bitshares.wallet_api_ptr->list_my_accounts();
   int n = 0;

   for(auto& account : accounts) {
      auto name = account.name;
      if(n == 0) strings.first_account_name = name;
      strings.accounts.Add(name);
      n++;
   }
   t_accounts->Set(strings.accounts);
   t_accounts->SetSelection(0);
   t_accounts->Enable(true);
}

void GWallet::DoModes()
{
   Dashboard *dashboard = new Dashboard(this);
   dashboard->SetSize(this->GetClientSize().x, this->GetClientSize().y);
   dashboard->Center();

   m_mgr.SetManagedWindow(dashboard);
   m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_TRANSPARENT_HINT|wxAUI_MGR_TRANSPARENT_DRAG);

   Wallet *wallet = new Wallet(this);
   panels.p_wallet = wallet;
   CreateWalletPane(wallet);

   Cli *cli = new Cli(this);
   panels.p_cli = cli;
   CreateCliPane(cli);

   Commands *commands = new Commands(this);
   panels.p_commands = commands;

   Info *information = new Info(this);
   About *about = new About(this);

   commands->notebook->AddPage(information, _("Blockchain information"));
   commands->notebook->AddPage(about, _("Software information"));

   CreateCommandsPane(commands);

   m_mgr.Update();
}

void GWallet::CreateWalletPane(Wallet* wallet)
{
   if(m_mgr.GetPane("Wallet").IsOk()) {
      wxWindow* wnd = m_mgr.GetPane("Wallet").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }

   wxAuiPaneInfo info;
   info.Top();
   info.PinButton();
   info.Caption(_("Wallet"));
   info.Position(0);
   auto width = this->GetClientSize().x/4;
   auto height = this->GetClientSize().y - this->GetClientSize().y/4;
   info.MinSize(width, height);
   info.BestSize(width, height);
   info.Resizable();
   info.dock_proportion = 2;
   info.PinButton();
   info.MaximizeButton();
   info.MinimizeButton();
   info.Name("Wallet");
   info.Dock();
   info.Movable();

   m_mgr.AddPane(wallet, info);
}

void GWallet::CreateCommandsPane(Commands* commands)
{
   if(m_mgr.GetPane("Commands").IsOk()) {
      wxWindow* wnd = m_mgr.GetPane("Commands").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }

   wxAuiPaneInfo info;
   info.Top();
   info.PinButton();
   info.Caption(_("Commands"));
   info.Position(1);
   auto width = this->GetClientSize().x - this->GetClientSize().x/4;
   auto height = this->GetClientSize().y - this->GetClientSize().y/4;
   info.MinSize(width, height);
   info.BestSize(width, height);
   info.Resizable();
   info.dock_proportion = 2;
   info.PinButton();
   info.MaximizeButton();
   info.MinimizeButton();
   info.Name("Commands");
   info.Dock();
   info.Movable();

   m_mgr.AddPane(commands, info);
}

void GWallet::CreateCliPane(Cli* cli)
{
   if(m_mgr.GetPane("Cli").IsOk()) {
      wxWindow* wnd = m_mgr.GetPane("Cli").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }

   wxAuiPaneInfo info;
   info.Bottom();
   info.PinButton();
   info.Caption(_("Cli"));
   info.Position(3);
   auto width = this->GetClientSize().x;
   info.MinSize(width, this->GetClientSize().y/4);
   info.BestSize(width, this->GetClientSize().y/4);
   info.Resizable();
   info.dock_proportion = 2;
   info.PinButton();
   info.MaximizeButton();
   info.MinimizeButton();
   info.Name("Cli");
   info.Dock();
   info.Movable();

   m_mgr.AddPane(cli, info);
}

void GWallet::LoadRegistrationWizardWidget()
{
   Registration *registration = new Registration(this);
   registration->RunWizard(registration->page1);
}

void GWallet::OnViewWelcome(wxCommandEvent& WXUNUSED(event))
{
   bool enable = false;
   if(menubar->IsChecked(XRCID("m_view_welcome"))) {
      main_panel->Raise();
   }
   else {
      main_panel->Lower();
      enable = true;
   }

   menubar->Enable(XRCID("m_view_commands"), enable);
   menubar->Enable(XRCID("m_view_wallet"), enable);
   menubar->Enable(XRCID("m_view_cli"), enable);
}

void GWallet::OnViewCommands(wxCommandEvent& WXUNUSED(event))
{
   if(menubar->IsChecked(XRCID("m_view_commands"))) {
      Commands *commands = new Commands(this);
      panels.p_commands = commands;
      CreateCommandsPane(commands);
   }
   else {
      wxWindow* wnd = m_mgr.GetPane("Commands").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }
   m_mgr.Update();
}

void GWallet::OnViewWallet(wxCommandEvent& WXUNUSED(event))
{
   if(menubar->IsChecked(XRCID("m_view_wallet"))) {
      Wallet *wallet = new Wallet(this);
      panels.p_wallet = wallet;
      CreateWalletPane(wallet);
   }
   else {
      wxWindow* wnd = m_mgr.GetPane("Wallet").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }
   m_mgr.Update();
}

void GWallet::OnViewCli(wxCommandEvent& WXUNUSED(event))
{
   if(menubar->IsChecked(XRCID("m_view_cli"))) {
      Cli *cli = new Cli(this);
      panels.p_cli = cli;
      CreateCliPane(cli);
   }
   else {
      wxWindow* wnd = m_mgr.GetPane("Cli").window;
      m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }
   m_mgr.Update();
}

void GWallet::OnPanelClose(wxAuiManagerEvent& event)
{
   const auto name = event.GetPane()->name;

   if(name == "Commands")
      menubar->Check(XRCID("m_view_commands"), false);
   if(name == "Wallet")
      menubar->Check(XRCID("m_view_wallet"), false);
   if(name == "Cli")
      menubar->Check(XRCID("m_view_cli"), false);
}

void GWallet::CreateEvents()
{
   Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNew));
   Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnOpen));
   Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSave));
   Connect(wxID_NETWORK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNetwork));
   Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnQuit));
   Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnAbout));

   Connect(XRCID("m_view_welcome"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnViewWelcome));
   Connect(XRCID("m_view_commands"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnViewCommands));
   Connect(XRCID("m_view_wallet"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnViewWallet));
   Connect(XRCID("m_view_cli"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnViewCli));

   m_mgr.Connect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(GWallet::OnPanelClose), NULL, this);

   Connect(XRCID("m_lang"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnChangeLanguage));
   Connect(XRCID("m_connect"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(XRCID("m_set_password"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSetPassword));
   Connect(XRCID("m_lock"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnLock));
   Connect(XRCID("m_unlock"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnUnlock));
   Connect(XRCID("m_import_key"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnImportKey));
   Connect(XRCID("m_import_balance"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnImportBalance));
   Connect(XRCID("m_disconnect"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnDisconnect));

   Connect(XRCID("t_connect"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(XRCID("t_disconnect"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnDisconnect));
   Connect(XRCID("t_lock"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnLock));
   Connect(XRCID("t_unlock"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnUnlock));
   Connect(XRCID("t_importkey"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnImportKey));

   Connect(XRCID("t_accounts"), wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAccount));
   Connect(XRCID("t_assets"), wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAsset));

   Connect(XRCID("new_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnNew));
   Connect(XRCID("open_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnOpen));
   Connect(XRCID("connect_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnConnect));
}

void GWallet::OnError(wxWindow* parent, wxString msg)
{
   wxMessageDialog dialog(parent, msg, _("Error"), wxOK|wxICON_ERROR);
   if (dialog.ShowModal() == wxID_OK)
      return;
}

void GWallet::OnChangeLanguage(wxCommandEvent& WXUNUSED(event))
{
   wxArrayInt languageCodes;
   wxArrayString languageNames;

   languageCodes.Add(wxLANGUAGE_CHINESE);
   languageNames.Add(_("Chinese"));
   languageCodes.Add(wxLANGUAGE_SPANISH);
   languageNames.Add(_("Spanish"));
   languageCodes.Add(wxLANGUAGE_ENGLISH);
   languageNames.Add(_("English"));
   int lang = wxGetSingleChoiceIndex(_("Select language:"), _("Language"), languageNames);
   if(lang != -1)
   {
      SelectLanguage(languageCodes[lang]);
   }
}

void GWallet::SelectLanguage(int lang)
{
   delete locale;
   locale = new wxLocale(lang);
   if(locale->Init(lang)) {
      config->Write("Lang", lang);
      config->Flush();

      wxMessageDialog dialog(NULL,
            _("Language changed, however you need to restart G-Wallet for changes to take effect"),
            _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
}

std::string GWallet::DoPrettyBalance(int precision, double balance)
{
   const auto divisor = pow(10, precision);
   stringstream pretty_balance;
   pretty_balance << fixed << std::setprecision(precision) << balance/divisor;
   return pretty_balance.str();
}

void GWallet::DoState() {

   menubar = this->GetMenuBar();

   if(state.is_noconfig) {
      menubar->Enable(XRCID("wxID_NEW"), true);
      menubar->Enable(XRCID("wxID_OPEN"), true);
      menubar->Enable(XRCID("wxID_SAVE"), false);
      menubar->Enable(XRCID("wxID_NETWORK"), false);

      menubar->Enable(XRCID("m_connect"), false);
      menubar->Enable(XRCID("m_disconnect"), false);
      menubar->Enable(XRCID("m_set_password"), false);
      menubar->Enable(XRCID("m_lock"), false);
      menubar->Enable(XRCID("m_unlock"), false);
      menubar->Enable(XRCID("m_import"), false);
      menubar->Enable(XRCID("m_import_key"), false);
      menubar->Enable(XRCID("m_import_balance"), false);

      toolbar->EnableTool(XRCID("t_connect"), false);
      toolbar->EnableTool(XRCID("t_disconnect"), false);
      toolbar->EnableTool(XRCID("t_lock"), false);
      toolbar->EnableTool(XRCID("t_unlock"), false);
      toolbar->EnableTool(XRCID("t_importkey"), false);

      connect_button->Enable(false);

      SetStatusText(_("No Config"), 0);
   }
   else if(state.is_connected) {
      menubar->Enable(XRCID("wxID_NEW"), true);
      menubar->Enable(XRCID("wxID_OPEN"), true);
      menubar->Enable(XRCID("wxID_SAVE"), true);
      menubar->Enable(XRCID("wxID_SAVE"), true);

      menubar->Enable(XRCID("m_connect"), false);
      menubar->Enable(XRCID("m_disconnect"), true);
      menubar->Enable(XRCID("m_setpassword"), false);
      menubar->Enable(XRCID("m_lock"), false);
      menubar->Enable(XRCID("m_unlock"), false);
      menubar->Enable(XRCID("m_import_key"), false);
      menubar->Enable(XRCID("m_import_balance"), false);

      toolbar->EnableTool(XRCID("t_connect"), false);
      toolbar->EnableTool(XRCID("t_disconnect"), true);
      toolbar->EnableTool(XRCID("t_lock"), false);
      toolbar->EnableTool(XRCID("t_unlock"), false);
      toolbar->EnableTool(XRCID("t_importkey"), false);

      menubar->Check(XRCID("m_view_welcome"), false);
      menubar->Check(XRCID("m_view_commands"), true);
      menubar->Check(XRCID("m_view_wallet"), true);
      menubar->Check(XRCID("m_view_cli"), true);

      menubar->Enable(XRCID("m_view_welcome"), true);
      menubar->Enable(XRCID("m_view_commands"), true);
      menubar->Enable(XRCID("m_view_wallet"), true);
      menubar->Enable(XRCID("m_view_cli"), true);

      toolbar->EnableTool(XRCID("t_accounts"), true);
      toolbar->EnableTool(XRCID("t_assets"), true);

      connect_button->Enable(false);

      if(state.is_new) {
         SetStatusText(_("Connected | New"));
         menubar->Enable(XRCID("m_set_password"), true);
      }
      else if(!state.is_account_linked) {
         SetStatusText(_("Connected | New"));
         menubar->Enable(XRCID("m_import_key"), false);
         menubar->Enable(XRCID("m_import_balance"), false);
         menubar->Enable(XRCID("m_unlock"), true);
      }
      else if(state.is_locked) {
         SetStatusText(_("Connected | Locked"));

         menubar->Enable(XRCID("m_set_password"), false);
         menubar->Enable(XRCID("m_lock"), false);
         menubar->Enable(XRCID("m_unlock"), true);
         menubar->Enable(XRCID("m_import_key"), false);
         menubar->Enable(XRCID("m_import_balance"), false);

         toolbar->EnableTool(XRCID("t_unlock"), true);
      }
      else if(state.is_unlocked) {
         SetStatusText(_("Connected | Unlocked"));

         menubar->Enable(XRCID("m_set_password"), false);
         menubar->Enable(XRCID("m_lock"), true);
         menubar->Enable(XRCID("m_unlock"), false);
         menubar->Enable(XRCID("m_import_key"), true);
         menubar->Enable(XRCID("m_import_balance"), true);

         toolbar->EnableTool(XRCID("t_lock"), true);
         toolbar->EnableTool(XRCID("t_unlock"), false);
         toolbar->EnableTool(XRCID("t_importkey"), true);
      }
      wxString server;
      if(config->Read("Server", &server)) {
         int widths[] = {GetClientSize().x - 25 - GetTextExtent(server).x, -1};
         SetStatusWidths(2, widths);
         SetStatusText(server, 1);
      }
   }
   else if(!state.is_connected) {

      toolbar->EnableTool(XRCID("t_accounts"), false);
      toolbar->EnableTool(XRCID("t_assets"), false);

      toolbar->EnableTool(XRCID("t_connect"), true);
      toolbar->EnableTool(XRCID("t_disconnect"), false);
      toolbar->EnableTool(XRCID("t_lock"), false);
      toolbar->EnableTool(XRCID("t_unlock"), false);
      toolbar->EnableTool(XRCID("t_importkey"), false);

      menubar->Enable(XRCID("m_connect"), true);
      menubar->Enable(XRCID("m_disconnect"), false);
      menubar->Enable(XRCID("m_set_password"), false);
      menubar->Enable(XRCID("m_lock"), false);
      menubar->Enable(XRCID("m_unlock"), false);
      menubar->Enable(XRCID("m_import_key"), false);
      menubar->Enable(XRCID("m_import_balance"), false);

      connect_button->Enable(true);

      SetStatusText(_("Disconnected"), 0);
   }
}

void GWallet::DoSearchAccount(const wxString& keyword, wxSearchCtrl& account_field)
{
   wxArrayString choices;
   auto findings = bitshares.database_api->lookup_accounts(keyword.ToStdString(), 100);
   for(auto f : findings)
      choices.Add(f.first);

   wxSingleChoiceDialog dialog(this, _("Accounts found"), _("Please select an account"), choices);
   if (dialog.ShowModal() == wxID_OK)
      account_field.SetValue(dialog.GetStringSelection());
}

void GWallet::DoSearchAsset(const wxString& keyword, wxSearchCtrl& asset_field)
{
   wxArrayString choices;
   auto findings = bitshares.database_api->list_assets(keyword.ToStdString(), 100);
   for(auto f : findings)
   {
      choices.Add(f.symbol);
   }

   wxSingleChoiceDialog dialog(this, _("Assets found"), _("Please select an asset"), choices);
   if (dialog.ShowModal() == wxID_OK)
      asset_field.SetValue(dialog.GetStringSelection());
}