//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
// Sample illustrating the TTabControl class
//----------------------------------------------------------------------------

#include "pch.h"
#include "resource.rh"
#include <owl/tabctrl.h>
#include <owl/notetab.h>
#include <owl/button.h>
#include <owl/groupbox.h>
#include <owl/radiobut.h>
#include <owl/checkbox.h>
#include <owl/inputdia.h>
#include <owl/framewin.h>

using namespace owl;

//
// Define a few constants
//
const int TabCtlID = 0x100;        // ID of the TabControl

//
// Template to toggle bits on and off
//
template <class T1, class T2>
inline void BitsOnOff(T1& value, T2 bitOn, T2 bitOff)
{
  value |= bitOn;
  value &= ~bitOff;
}

class TMainWindow 
  : public TFrameWindow 
{
public:

  TMainWindow(TWindow* clientWnd, bool shrinkToClient = false);

protected:

  void AdjustTabClient();

  // Message Handlers
  //
  void EvSize(uint sizeType, const TSize& size);
  void TabSelChange(TNotify&);
  bool TabSelChanging(TNotify&);
  void TabKeyDown(TTabKeyDown&);

  void CmNewTab();
  void CmAddItem();
  void CmRemoveItem();
  void CeRemoveItem(TCommandEnabler& ce);
  void CmDeleteAll();
  void CeDeleteAll(TCommandEnabler& ce);
  void CmChangeItem();
  void CeChangeItem(TCommandEnabler& ce);
  void CmInfo();
  void CmVetoSelChanging();
  void CeVetoSelChanging(TCommandEnabler& ce);
  void CmShowNoteTab();

private:

  TTabControl*  Tab;
  bool VetoSelChanging;
  TWindow* Client;

  DECLARE_RESPONSE_TABLE(TMainWindow);
};

DEFINE_RESPONSE_TABLE1(TMainWindow, TFrameWindow)
  EV_WM_SIZE,

  EV_TCN_SELCHANGE(TabCtlID, TabSelChange),
  EV_TCN_SELCHANGING(TabCtlID, TabSelChanging),
  EV_TCN_KEYDOWN(TabCtlID, TabKeyDown),

  EV_COMMAND(CM_NEWTAB, CmNewTab),
  EV_COMMAND(CM_ADDITEM, CmAddItem),
  EV_COMMAND(CM_REMOVEITEM, CmRemoveItem),
  EV_COMMAND_ENABLE(CM_REMOVEITEM, CeRemoveItem),
  EV_COMMAND(CM_DELETEALL, CmDeleteAll),
  EV_COMMAND_ENABLE(CM_DELETEALL, CeDeleteAll),
  EV_COMMAND(CM_CHANGEITEM, CmChangeItem),
  EV_COMMAND_ENABLE(CM_CHANGEITEM, CeChangeItem),
  EV_COMMAND(CM_INFO, CmInfo),
  EV_COMMAND(CM_VETOSELCHANGING, CmVetoSelChanging),
  EV_COMMAND_ENABLE(CM_VETOSELCHANGING, CeVetoSelChanging),
END_RESPONSE_TABLE;

TMainWindow::TMainWindow(TWindow* clientWnd, bool shrinkToClient)
  : TFrameWindow(0, "TTabControl Test", clientWnd, shrinkToClient),
  VetoSelChanging(false), 
  Tab(dynamic_cast<TTabControl*>(clientWnd)), 
  Client(0)
{}

void
TMainWindow::AdjustTabClient()
{
  // Retrieve the window rectangle of the tab control.
  //
  TRect rect;
  Tab->GetWindowRect(rect);

  // NOTE: GetWindowRect returns screen coordinates. We'll need
  // to have the coordinates relative to the frame window,
  // the parent of both the tab and client window
  //
  ::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT) &rect, 2);

  // Ask the tab for it's 'client-area' based in it window location.
  //
  Tab->AdjustRect(false, rect);

  // Move the Client window.
  //
  if (Client && Client->GetHandle())
    Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);
}

void
TMainWindow::EvSize(uint sizeType, const TSize& size)
{
  // Let frame resize the client
  //
  TFrameWindow::EvSize(sizeType, size);

  // Adjust the edit control (which is kind of the client of the
  // tab control)...
  //
  AdjustTabClient();
}

void
TMainWindow::TabSelChange(TNotify& /*nm*/)
{
  // NOP
}

bool
TMainWindow::TabSelChanging(TNotify& /*nm*/)
{
  return VetoSelChanging ? 1 : 0;
}

void
TMainWindow::TabKeyDown(TTabKeyDown& /*tk*/)
{
  // NOP
}

//
// CmNewTab() creates a dialog which allows the user to select the type
// of TabControl to be created. It then proceeds to create the control
// and makes the new control the client of the application's frame window.
//
void
TMainWindow::CmNewTab()
{
  // Create/Initialize a structure to set and retrieve data.
  //
  struct
  {
    uint16 buttonStyle;
    uint16 defStyle;
    uint16 singleLine;
    uint16 multipleLine;
  }
  dlgData = {0, 1, 1, 0};

  // Create C++ objects representing the Dialog and its controls.
  //
  TDialog* dlg = new TDialog(this, IDD_NEWTAB);
  new TRadioButton(dlg, IDC_RADIOBUTTON1);
  new TRadioButton(dlg, IDC_RADIOBUTTON2);
  new TRadioButton(dlg, IDC_RADIOBUTTON3);
  new TRadioButton(dlg, IDC_RADIOBUTTON4);

  // Set the transfer buffer and execute the dialog.
  //
  dlg->SetTransferBuffer(&dlgData);
  if (dlg->Execute() == IDOK)
  {
    // Create a new Tab Control.
    //
    TTabControl* tab = new TTabControl(0, TabCtlID, 0, 0, 0, 0);

    // Convert selection into style bits.
    //
    if (dlgData.defStyle)
      BitsOnOff(tab->Attr.Style, TCS_TABS, TCS_BUTTONS);
    if (dlgData.buttonStyle)
      BitsOnOff(tab->Attr.Style, TCS_BUTTONS, TCS_TABS);
    if (dlgData.singleLine)
      BitsOnOff(tab->Attr.Style, TCS_SINGLELINE, TCS_MULTILINE);
    if (dlgData.multipleLine)
      BitsOnOff(tab->Attr.Style, TCS_MULTILINE, TCS_SINGLELINE);

    // Cleanup prior client window.
    //
    TWindow* oldClient = SetClientWindow(tab);
    Tab = tab;
    if (oldClient)
    {
      if (oldClient->GetHandle())
        oldClient->Destroy();
      delete oldClient;
    }
  }
}

//
// This function prompts the user for a label and creates a new tab item.
//
void
TMainWindow::CmAddItem()
{
  TInputDialog dlg(this, "New Item", "Enter Item Label");
  if (dlg.Execute() == IDOK)
  {
    if (!dlg.GetBuffer()[0]) return;

    // Add a new item to the tabcontrol.
    //
    TTabItem tabItem(dlg.GetBuffer());
    Tab->Add(tabItem);

    // Adjust Client in case the Tab items got rearranged.
    //
    AdjustTabClient();
  }
}

void
TMainWindow::CeRemoveItem(TCommandEnabler& ce)
{
  ce.Enable((Tab && Tab->GetSel() != -1) ? true : false);
}


//
// This methods offers the user a list of the tab items and removes
// the selected one (if one is selected via the picklist dialog).
//
void
TMainWindow::CmRemoveItem()
{
  TInputDialog dlg(this, "Remove Item", "Enter Item Index");
  if (dlg.Execute() == IDOK && dlg.GetCaption()[0])
  {
    int index = atoi(dlg.GetCaption());

    // Remove item from tabcontrol.
    //
    if (index < Tab->GetCount() && index >= 0)
    {
      Tab->Delete(index);

      // Adjust Client in case the Tab items got rearranged.
      //
      AdjustTabClient();
    }
    else
      MessageBox("Invalid index specified", "INFO");
  }
}

void
TMainWindow::CmChangeItem()
{
  int index = Tab->GetSel();
  if (index == -1) return;

  char label[80] = {};
  TTabItem item(*Tab, index, TCIF_TEXT, COUNTOF(label), label);

  tostringstream s;
  s << "New label of (" << label << ")";
  TInputDialog dlg(this, "Change Item", s.str(), label, COUNTOF(label));
  if (dlg.Execute() == IDOK && label[0])
  {
    Tab->SetItem(index, item);

    // Adjust Client in case the Tab items got rearranged.
    //
    AdjustTabClient();
  }
}

void
TMainWindow::CeChangeItem(TCommandEnabler& ce)
{
  ce.Enable((Tab && Tab->GetSel() != -1) ? true : false);
}

void
TMainWindow::CmDeleteAll()
{
  Tab->DeleteAll();

  // Adjust Client in case the Tab items got rearranged.
  //
  AdjustTabClient();
}

void
TMainWindow::CeDeleteAll(TCommandEnabler& ce)
{
  ce.Enable((Tab && Tab->GetCount()) ? true : false);
}

void
TMainWindow::CmVetoSelChanging()
{
  VetoSelChanging = !VetoSelChanging;
}

void
TMainWindow::CeVetoSelChanging(TCommandEnabler& ce)
{
  ce.SetCheck(VetoSelChanging ? TCommandEnabler::Checked :
    TCommandEnabler::Unchecked);
}

void TMainWindow::CmInfo()
{
  tostringstream s;
  s << "Count: " << Tab->GetCount()
    << ", RowCount: " << Tab->GetRowCount()
    << ", Sel: " << Tab->GetSel()
    << ", ImageList: 0x" << std::hex << static_cast<void*>(Tab->GetImageList());
  MessageBox(s.str(), "INFO");
}

//
// A simple Application Object which simply defines InitMainWindow
// to initialize the application's frame and client windows.
//
class TSampleApp 
  : public TApplication 
{
protected:

  virtual void InitMainWindow(); // override
};

void
TSampleApp::InitMainWindow()
{
  // Create tab and main window.
  // Use tab as client of the main window.
  //
  TTabControl* tab = new TTabControl(0, TabCtlID, 0, 0, 0, 0);
  TMainWindow* w = new TMainWindow(tab);

  TWindowAttr& a = w->GetWindowAttr();
  a.W = 320;
  a.H = 240;

  SetMainWindow(w);
  w->AssignMenu(IDM_APPMENU);
}

int
OwlMain(int, char* []) // argc, argv
{
  return TSampleApp().Run();
}
