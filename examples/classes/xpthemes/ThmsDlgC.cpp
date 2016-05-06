//----------------------------------------------------------------------------
//  Project Themes
//  
//  (C) 2007 Sebastian Ledesma
//
//  SUBSYSTEM:    Themes Application
//  FILE:         thmsdlgc.cpp
//  AUTHOR:       Sebastian Ledesma
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TThemesDlgClient (TDialog).
//
//----------------------------------------------------------------------------

#include <owl/pch.h>

using namespace owl;

#include "themsapp.h"
#include "thmsdlgc.h"
#include "moredilg.h"


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TThemesDlgClient, TDialog)
//{{TThemesDlgClientRSP_TBL_BEGIN}}
  EV_WM_TIMER,
  EV_BN_CLICKED(IDC_BUTTON1, OnButton1),
//{{TThemesDlgClientRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TThemesDlgClient Implementation}}


//--------------------------------------------------------
// TThemesDlgClient
// ~~~~~~~~~~
// Construction/Destruction handling.
//

TThemesDlgClient::TThemesDlgClient(TWindow* parent, TResId resId, TModule* module)
:
  TDialog(parent, resId, module)
{
  pComboBox1 = new TComboBox(this, IDC_COMBOBOX1, 1);
  pListBox1 = new TListBox(this, IDC_LISTBOX1);
  pProgress = new TProgressBar(this, IDC_PROGRESSBAR1);
  pGauge = new TGauge(this, IDC_PROGRESSBAR2);
  //pGauge = new TGauge(this, _T("gauge"), -1, 100, 190, 160, 22, true);
  pTabControl = new TTabControl(this, IDC_TABCONTROL1);
  SetBkgndColor(RGB(255,225, 132));
}


TThemesDlgClient::~TThemesDlgClient()
{
  Destroy();

  // INSERT>> Your destructor code here.
}

void TThemesDlgClient::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.
  pComboBox1->AddString(_T("TComboBox Item 1"));
  pComboBox1->AddString(_T("TComboBox Item 2"));
  pComboBox1->AddString(_T("TComboBox Item 3"));
  pComboBox1->AddString(_T("TComboBox Item 4"));
  pComboBox1->SetSelIndex(2);

  pListBox1->AddString(_T("TListBox Item 1"));
  pListBox1->AddString(_T("TListBox Item 2"));
  pListBox1->AddString(_T("TListBox Item 3"));
  pListBox1->AddString(_T("TListBox Item 4"));
  pListBox1->AddString(_T("TListBox Item 5"));
  pListBox1->AddString(_T("TListBox Item 6"));
  pListBox1->AddString(_T("TListBox Item 7"));
  pListBox1->AddString(_T("TListBox Item 8"));
  pListBox1->AddString(_T("TListBox Item 9"));
  pListBox1->SetSelIndex(1);

  pTabControl->Add(_T("Tab 1"));
  pTabControl->Add(_T("Tab 2"));
  pTabControl->Add(_T("Tab 3"));

  SetTimer(1, 20); //timer 1 set each 10 milliseconds

}

void TThemesDlgClient::CleanupWindow()
{
  TDialog::CleanupWindow();

  KillTimer(1);
}

void TThemesDlgClient::EvTimer(uint timerId)
{
  TDialog::EvTimer(timerId);

  int value=pGauge->GetValue();
  value++;
  if (value==100)
    value=0;
  pGauge->SetValue(value);
  pProgress->SetValue(value);
}


void TThemesDlgClient::OnButton1()
{
  TMoreDialog* xTMoreDialog = new TMoreDialog(this);
  xTMoreDialog->Execute();
  delete xTMoreDialog;
}



