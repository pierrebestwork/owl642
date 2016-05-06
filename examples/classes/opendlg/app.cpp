//---------------------------------------------------------------------------

#pragma hdrstop

#include "app.h"
#include "frame.h"

using namespace owl;

TMyApplication::TMyApplication()
: TApplication(_T("Open File dialog share violation options"))
{
  // ...other initialization here...
}

void TMyApplication::InitMainWindow()
{
  TMyFrame* frame = new TMyFrame(0, GetName()); // Use the application name as title.
  SetMainWindow(frame);
}




int OwlMain(int argc, _TCHAR* argv[])
{
  return TMyApplication().Run();
}

