#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Cli : public wxPanel
{
public:
   Cli(GWallet* gwallet);
   ~Cli();

   Cli(wxWindow *parent=NULL) {
      InitWidgetsFromXRC((wxWindow *)parent);
   }
   void OnCliCommand(wxCommandEvent& event);

   GWallet* p_GWallet;
   wxComboBox* command;

protected:
   wxButton* run;
   wxButton* clear;
   wxTextCtrl* output;
   wxTextFile commands_log;

private:
   void OnCliClear(wxCommandEvent& event);
   void ReadCommands();

   void InitWidgetsFromXRC(wxWindow *parent) {
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Cli"), wxT("wxPanel"));
      command = XRCCTRL(*this,"command",wxComboBox);
      run = XRCCTRL(*this,"run",wxButton);
      clear = XRCCTRL(*this,"clear",wxButton);
      output = XRCCTRL(*this,"output",wxTextCtrl);
   }
};
