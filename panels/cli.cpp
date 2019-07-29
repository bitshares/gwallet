#include "../include/panels/cli.hpp"

Cli::Cli(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   wxString log_file = wxT("cli_commands_log.txt");
   wxTextFile logs(log_file);
   if(!logs.Exists()) {
      logs.Create();
   }
   logs.Close();
   commands_log.Open(log_file);

   ReadCommands();

   Connect(XRCID("command"), wxEVT_TEXT_ENTER, wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   Connect(XRCID("run"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   Connect(XRCID("clear"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cli::OnCliClear), NULL, this);
}

Cli::~Cli()
{
   commands_log.Close();
}

void Cli::OnCliCommand(wxCommandEvent& WXUNUSED(event))
{
   const auto line = command->GetValue();

   commands_log.InsertLine(line, 0);
   commands_log.Write();
   command->Insert(line, 0);

   try {
      const fc::variants line_variants = fc::json::variants_from_string(line.ToStdString());
      const auto command = line_variants[0].get_string();
      const auto arguments_variants = fc::variants( line_variants.begin()+1,line_variants.end());
      const auto response = p_GWallet->bitshares.wallet_cli->receive_call(p_GWallet->bitshares.api_id, command, arguments_variants);
      auto res = fc::json::to_pretty_string(response);

      boost::replace_all(res, "\\n", "\n");

      std::string prompt = "";
      if( p_GWallet->bitshares.wallet_api_ptr->is_new() )
      {
         prompt = "new >>> ";
      }
      else
      {
         if( p_GWallet->bitshares.wallet_api_ptr->is_locked() ) {
            prompt = "locked >>> ";
         }
         else {
            prompt = "unlocked >>> ";
         }
      }

      output->AppendText(prompt + line + "\n");
      output->AppendText(res + "\n");
      output->AppendText(prompt + "\n");
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}

void Cli::OnCliClear(wxCommandEvent& WXUNUSED(event))
{
   output->SetValue("");
}

void Cli::ReadCommands()
{
   while(!commands_log.Eof())
   {
      command->Append(commands_log.GetNextLine());
   }
}

void Cli::DoCommand(string command)
{
   p_GWallet->panels.p_cli->command->SetValue(command);
   wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
   p_GWallet->panels.p_cli->OnCliCommand(event);
}
