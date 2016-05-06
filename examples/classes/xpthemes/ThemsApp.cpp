//----------------------------------------------------------------------------
//  Project Themes
//  
//  (C) 2007 Sebastian Ledesma
//
//  SUBSYSTEM:    Themes Application
//  FILE:         themsapp.cpp
//  AUTHOR:       Sebastian Ledesma
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TThemesApp (TApplication).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

#include <owl/buttonga.h>
#include <owl/statusba.h>
#include <owl/commctrl.h>
#include <stdio.h>

using namespace owl;

#include "themsapp.h"
#include "thmsdlgc.h"                        // Definition of client class.


//{{TThemesApp Implementation}}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TThemesApp, TApplication)
//{{TThemesAppRSP_TBL_BEGIN}}
  EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
//{{TThemesAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TThemesApp
//
TThemesApp::TThemesApp() : TApplication(_T("Themes"))
{

  // INSERT>> Your constructor code here.
}


TThemesApp::~TThemesApp()
{
  // INSERT>> Your destructor code here.
}


//--------------------------------------------------------
// TThemesApp
// ~~~~~
// Application intialization.
//
void TThemesApp::InitMainWindow()
{
  if (nCmdShow != SW_HIDE)
    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  TSDIDecFrame* frame = new TSDIDecFrame(0, GetName(), 0, true);
  frame->SetFlag(wfShrinkToClient);

  // Assign icons for this application.
  //
  frame->SetIcon(this, IDI_SDIAPPLICATION);
  frame->SetIconSm(this, IDI_SDIAPPLICATION);

  SetMainWindow(frame);

  // We use InitCommonControls to enable XP themes.
  //
  TCommCtrl::Dll()->InitCommonControls();
}



//{{TSDIDecFrame Implementation}}


TSDIDecFrame::TSDIDecFrame(TWindow* parent, LPCTSTR title, TWindow* clientWnd, bool trackMenuSelection, TModule* module)
:
  TDecoratedFrame(parent, title, !clientWnd ? new TThemesDlgClient(0) : clientWnd, trackMenuSelection, module)
{
  // INSERT>> Your constructor code here.

}


TSDIDecFrame::~TSDIDecFrame()
{
  // INSERT>> Your destructor code here.

}


void TSDIDecFrame::SetupWindow()
{
  TDecoratedFrame::SetupWindow();
  TRect  r;
  GetWindowRect(r);

  r.bottom += 30;
  SetWindowPos(0, r, SWP_NOZORDER | SWP_NOMOVE);

  // INSERT>> Your code here.

}


//--------------------------------------------------------
// TThemesApp
// ~~~~~~~~~~~
// Menu Help About Themes command
//
void TThemesApp::CmHelpAbout()
{
}



int OwlMain(int , _TCHAR* [])
{
  TThemesApp   app;
  return app.Run();
}
