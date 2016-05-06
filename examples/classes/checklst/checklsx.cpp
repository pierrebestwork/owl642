#if defined(BI_COMP_BORLANDC)
#include <condefs.h>
USERC("checklst.rc");
#endif

//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/checklst.h>
#include <stdio.h>

using namespace owl;


const int CheckListId = 100;
const int NumItems = 15;

_TCHAR Buffer[4096];

//
// class CheckListXWindow
// ~~~~~ ~~~~~~~~~~~~~~~~
class TCheckListXWindow : public TWindow {
  public:
    TCheckListXWindow();
   ~TCheckListXWindow();

    void  SetupWindow();
    void  CleanupWindow();

    bool  CanClose();

  private:
    TCheckList* CheckList;
    TCheckListItem* Items;

  DECLARE_RESPONSE_TABLE(TCheckListXWindow);
};


//
// CheckListXWindow constructor
//
TCheckListXWindow::TCheckListXWindow()
:
  TWindow(0, 0, 0)
{
  Items = new TCheckListItem[NumItems];

  for (int i = 0; i < NumItems; i++) {
    _TCHAR buffer[20];
    wsprintf(buffer, _T("Item %d"), i);
    Items[i].SetText(buffer);
  }
  Items[0].Toggle();
  Items[1].SetIndeterminate();
  Items[2].SetThreeStates(true);

  CheckList = new TCheckList(this, CheckListId, 10, 10, 400, 200, Items, NumItems);
}


//
// CheckListXWindow destructor
//
TCheckListXWindow::~TCheckListXWindow()
{
  delete[] Items;
}


//
// SetupWindow
//
void
TCheckListXWindow::SetupWindow()
{
  TWindow::SetupWindow();
  // put new stuff here
  //

}

//
//
//
bool
TCheckListXWindow::CanClose()
{
  ::Buffer[0] = 0;
  _tcscpy(::Buffer, _T("You've selected\r\n"));
  for (int i = 0; i < NumItems; i++) {
    if (Items[i].IsChecked() || Items[i].IsIndeterminate()) {
      _tcscat(::Buffer, Items[i].GetText().c_str());
      if (Items[i].IsIndeterminate())
        _tcscat(::Buffer, _T(" (ind)"));
      _tcscat(::Buffer, _T("\r\n"));
    }
  }

  return MessageBox(::Buffer, _T("Okay to close?"), MB_OKCANCEL) == IDOK;
}


//
// CleanupWindow
//
void
TCheckListXWindow::CleanupWindow()
{
  // put new stuff here
  //
  TWindow::CleanupWindow();
}


//
// Response table for CheckListXWindow
//
DEFINE_RESPONSE_TABLE1(TCheckListXWindow, TWindow)
END_RESPONSE_TABLE;


//
// class CheckListXApplication
// ~~~~~ ~~~~~~~~~~~~~~~~~~~~~
class TCheckListXApp : public TApplication {
  public:
    TCheckListXApp()
    {
    }
   ~TCheckListXApp()
    {
    }

    void InitMainWindow()
    {
      TFrameWindow* frame = new TFrameWindow(0, _T("CheckListX application"),
        new TCheckListXWindow);
      SetMainWindow(frame);
    }
};


//
// OwlMain
//
int
OwlMain(int /*argc*/, _TCHAR* /*argv*/ [])
{
  TCheckListXApp app;
  return app.Run();
}

#if defined(BI_COMP_BORLANDC)
#define WinMain
#endif
