//---------------------------------------------------------------------------

#pragma hdrstop

#include <owl/listbox.h>

#include <owl/window.rh>
#include <owl/editfile.rh>

#include "frame.h"
#include "opendlg.h"
#include "resources.h"

using namespace owl;

DEFINE_RESPONSE_TABLE1(TMyFrame, TFrameWindow)

  EV_COMMAND(CM_ABOUT, CmAbout),

  EV_COMMAND(CM_FILEOPEN, CmFileOpen),

  EV_COMMAND(CM_SHAREWARN, CmShareWarn),
  EV_COMMAND(CM_SHARENOWARN, CmShareNoWarn),
  EV_COMMAND(CM_SHAREFALLTHROUGH, CmShareFallThrough),

  EV_COMMAND(CM_EXPLORERSTYLE, CmExplorer),

  EV_COMMAND_ENABLE(CM_SHAREWARN, CeShareWarn),
  EV_COMMAND_ENABLE(CM_SHARENOWARN, CeShareNoWarn),
  EV_COMMAND_ENABLE(CM_SHAREFALLTHROUGH, CeShareFallThrough),

  EV_COMMAND_ENABLE(CM_EXPLORERSTYLE, CeExplorer),

END_RESPONSE_TABLE;

using namespace owl;

TMyFrame::TMyFrame(TWindow* parent, LPCTSTR title)
: TFrameWindow(parent, title, new TListBox(0, 1, 0, 0, 0, 0)) // Always call the base class constructor.
{
  share = OFN_SHAREWARN;
  explorer = true;

  TListBox* lb = dynamic_cast<TListBox*>(GetClientWindow());

  if (lb != 0)
  {
    lb->Attr.Style &= ~LBS_SORT;
  }

}

TMyFrame::~TMyFrame()
{
}

void TMyFrame::SetupWindow()
{
  TFrameWindow::SetupWindow(); // Always call the base class function.

  AssignMenu(TResId(MAIN_MENU));
}

void TMyFrame::CleanupWindow()
{
  TFrameWindow::CleanupWindow(); // Always call the base class function.
}

void TMyFrame::CmAbout()
{
  MessageBox(_T("Demonstrate the different options for the share violation handling in TOpenFileDialog:\n")
    _T("  OFN_SHAREWARN - shows a message that the file is already opened\n")
    _T("  OFN_SHARENOWARN - does not allow the file to be selected in the dialog\n")
    _T("  OFN_SHAREFALLTHROUGH - allows the file to be selected in the dialog. Attempt to open the file will fail with error\n")
    _T("\n")
    _T("To test the behavior, first open any file, then select one of the options and try to open the same file again."));
}

void TMyFrame::CmFileOpen()
{
  TOpenSaveDialog::TData data;

  data.SetFilter(_T("All Files (*.*)|*.*|"));

  if (!explorer)
  {
    data.Flags &= ~OFN_EXPLORER;
  }

  TOpenDialog dlg(this, data, share);
  if (dlg.Execute() == IDOK)
  {
    HANDLE hFile = CreateFile(data.FileName,
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
      owl::tstring error = TSystemMessage().SysMessage();

      ShowMessage(tstring(_T("Cannot open file ")) + data.FileName + _T(". Error: ") + error);
    }
    else
    {
      ShowMessage(tstring(_T("File ")) + data.FileName + _T(" opened"));
    }
  }
}

void TMyFrame::CmShareWarn()
{
  share = OFN_SHAREWARN;
}

void TMyFrame::CmShareNoWarn()
{
  share = OFN_SHARENOWARN;
}

void TMyFrame::CmShareFallThrough()
{
  share = OFN_SHAREFALLTHROUGH;
}

void TMyFrame::CmExplorer()
{
  explorer = !explorer;
}

void TMyFrame::CeShareWarn(owl::TCommandEnabler& ce)
{
 ce.SetCheck(share == OFN_SHAREWARN ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyFrame::CeShareNoWarn(owl::TCommandEnabler& ce)
{
 ce.SetCheck(share == OFN_SHARENOWARN ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyFrame::CeShareFallThrough(owl::TCommandEnabler& ce)
{
 ce.SetCheck(share == OFN_SHAREFALLTHROUGH ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyFrame::CeExplorer(owl::TCommandEnabler& ce)
{
 ce.SetCheck(explorer ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TMyFrame::ShowMessage(const owl::tstring& message)
{
  TListBox* lb = dynamic_cast<TListBox*>(GetClientWindow());

  if (lb != 0)
  {
    lb->AddString(message);
  }
}

