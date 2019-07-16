#include "../include/panels/voteforwitness.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

VoteForWitness::VoteForWitness(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   SetScrollRate(1,1);

   voting_account->Append(p_GWallet->strings.accounts);
   voting_account->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(VoteForWitness::OnOk));
   Connect(XRCID("witness"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(VoteForWitness::OnSearchWitness), NULL, this);

}

void VoteForWitness::OnSearchWitness(wxCommandEvent& event)
{
   p_GWallet->DoSearchWitness(event.GetString(), *witness);
}

void VoteForWitness::OnOk(wxCommandEvent& WXUNUSED(event))
{

   const auto _voting_account = voting_account->GetValue().ToStdString();
   const auto _witness = witness->GetValue().ToStdString();
   auto _approve = true;
   if(approve->GetSelection() == 1) _approve = false;

   signed_transaction result_obj;
   string _broadcast = "false";
   if(broadcast->IsChecked())
      _broadcast = "true";

   wxAny response;

   p_GWallet->panels.p_commands->Wait();

   if(cli->IsChecked())
   {
      stringstream command;
      command << "vote_for_witness " << _voting_account << " " << _witness + " " << std::boolalpha << _approve << " " << _broadcast;
      p_GWallet->panels.p_cli->DoCommand(command.str());
      p_GWallet->DoAssets(_voting_account);
   }
   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->vote_for_witness(_voting_account, _witness, _approve, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]),
                  _("Confirm vote for witness?"), wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               wxTheApp->Yield(true);
               result_obj = p_GWallet->bitshares.wallet_api_ptr->vote_for_witness(_voting_account, _witness, _approve, true);
               p_GWallet->DoAssets(_voting_account);
            }
            response = result_obj;
            new VoteForWitnessResponse(p_GWallet, response);
         }
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(this, e.to_detail_string());
      }
   }
}

VoteForWitnessResponse::VoteForWitnessResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
   SetScrollRate(1,1);

   gwallet->panels.p_commands->DoSignedTranactionResponse(response_tree, any_response.As<signed_transaction>());
   gwallet->panels.p_commands->notebook->AddPage(this, _("Vote for witness response"), true);
}
