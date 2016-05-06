//
// OWLNext WM_COPYDATA Demo
// This code demonstrates sending and receiving the WM_COPYDATA message.
//
// Copyright (C) 2012 Vidar Hasfjord 
// Distributed under the OWLNext License (see http://owlnext.sourceforge.net).

#include <owl/applicat.h>
#include <owl/framewin.h>
#include "resource.h"

using namespace owl;

//
// Data to be copied between windows/applications
//
struct TCopyData
{
  tchar Message[256];
};

//
// This enum defines the types of data this application knows about.
//
enum TCopyDataId
{
  cdidCopyData = 423141 // TCopyData message (random id to reduce likelihood of clashes)
};

//
// Test application
//
class TCopyDataApp
  : public TApplication
{
public:

  TCopyDataApp()
    : TApplication(_T("WM_COPYDATA Demo"))
  {}

protected:

  void InitMainWindow()
  {
    TFrameWindow* f = new TFrameWindow(0, GetName());
    f->SetMenuDescr(TMenuDescr(IDM_MAIN));
    f->SetWindowPos(0, 0, 0, 400, 200, SWP_NOMOVE);
    SetMainWindow(f);
  }

  void CmCopyData()
  {
    TCopyData d = {_T("Hello! This WM_COPYDATA message was sent from application to itself.")};
    COPYDATASTRUCT c = {cdidCopyData, sizeof d, &d};
    TResult r = GetMainWindow()->SendMessage(WM_COPYDATA, 
      reinterpret_cast<TParam1>(GetMainWindow()->GetHandle()),
      reinterpret_cast<TParam2>(&c));
    tstring m = r ? _T("The message was handled.") : _T("The message was not handled.");
    GetMainWindow()->MessageBox(m, _T("WM_COPYDATA Sender"));
  }

#if defined(OWL5_COMPAT)

  void EvCopyData(HWND sender, PCOPYDATASTRUCT p)
  {EvCopyData(sender, *p);}

#endif

  //
  // Returns true if the message is handled.
  //
  bool EvCopyData(HWND sender, const COPYDATASTRUCT& c)
  {
    PRECONDITION(sender == GetMainWindow()->GetHandle());
    const tchar title[] = _T("WM_COPYDATA Receiver");
    switch (c.dwData)
    {
    case cdidCopyData:
      {
        CHECK(c.cbData == sizeof TCopyData);
        const TCopyData& d = *reinterpret_cast<const TCopyData*>(c.lpData);
        GetMainWindow()->MessageBox(d.Message, title);
        return true;
      }
    default:
      {
        GetMainWindow()->MessageBox(_T("Unknown message."), title);
        return false;
      }
    }
  }

  DECLARE_RESPONSE_TABLE(TCopyDataApp);
};

DEFINE_RESPONSE_TABLE1(TCopyDataApp, TApplication)
  EV_COMMAND(CM_COPYDATA, CmCopyData),
  EV_WM_COPYDATA,
END_RESPONSE_TABLE;

int OwlMain(int, tchar* [])
{
  return TCopyDataApp().Run();
}
