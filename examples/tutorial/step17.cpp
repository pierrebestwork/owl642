//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1994 by Borland International
//   Tutorial application -- step17.cpp
//   Enhanced Embed/Linking/Automation Server/Container example
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/appdict.h>
#include <owl/dialog.h>
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <owl/listbox.h>
#include <owl/statusba.h>
#include <owl/docmanag.h>
#if OWLVersion >= 0x600
#include <ocf/olemdifr.h>
#include <ocf/oledoc.h>
#include <ocf/oleview.h>
#else
#include <owl/olemdifr.h>
#include <owl/oledoc.h>
#include <owl/oleview.h>
#endif
#include <classlib/arrays.h>
#include <ocf/automacr.h>
#include <stdlib.h>
#include <string.h>

using namespace owl;
using namespace ocf;

#include "step17.rc"
#include "step17dv.h"
#include "step17.h"

DEFINE_APP_DICTIONARY(AppDictionary);
static TPointer<TOcRegistrar> Registrar;

// Registration
//
REGISTRATION_FORMAT_BUFFER(100)

BEGIN_REGISTRATION(AppReg)
  REGDATA(clsid,      "{5E4BD340-8ABC-101B-A23B-CE4E85D07ED2}")
  REGDATA(progid,     "DrawPad.Application.17")
  REGDATA(description,"DrawPad AutoContServer Application")
  REGDATA(appname,    "DrawPad AutoContServer")
//REGDATA(debugger,   "tdw")
  REGDATA(cmdline,    "/automation")
  REGDATA(usage,      ocrMultipleUse)
END_REGISTRATION

//
// TDrawApp
//
DEFINE_RESPONSE_TABLE1(TDrawApp, TApplication)
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_WM_DROPFILES,
  EV_COMMAND(CM_ABOUT, CmAbout),
END_RESPONSE_TABLE;

DEFINE_AUTOAGGREGATE(TDrawApp, OcApp->Aggregate)
  EXPOSE_PROPRW(Visible, TAutoBool,    "Visible",     "Main window shown", 0)
  EXPOSE_METHOD(NewDoc,  TDrawDocument,"NewDocument", "Create new document", 0)
  EXPOSE_METHOD(OpenDoc, TDrawDocument,"OpenDocument","Open existing document", 0)
   REQUIRED_ARG(         TAutoString,  "Name")
  EXPOSE_PROPRO(AppName, TAutoString,  "Name",        "Application name", 0)
  EXPOSE_PROPRO(FullName,TAutoString,  "FullName",    "Complete path to application", 0)
  EXPOSE_APPLICATION(TDrawApp,         "Application", "Application object", 0)
  EXPOSE_QUIT(                         "Quit",        "Shutdown application", 0)
END_AUTOAGGREGATE(TDrawApp,tfAppObject|tfCanCreate,"TDrawApp","Application class", 0)

const char far*
TDrawApp::GetPath()
{
  static char buf[_MAX_PATH];
  GetModuleFileName(buf, sizeof(buf)-1);
  return buf;
}

bool
TDrawApp::GetShow()
{
  TFrameWindow* frame = GetMainWindow();
  return (frame && frame->IsWindow() && frame->IsWindowVisible());
}

void
TDrawApp::SetShow(bool visible)
{
  TFrameWindow* frame = GetMainWindow();
  if (frame && frame->IsWindow()) {
    unsigned flags = visible ? SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW
                : SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW;
    frame->SetWindowPos(HWND_TOP, 0,0,0,0, flags);
  }
}

const char  DocContent[] = "All";
const char  DrawPadFormat[] = "DrawPad";

BEGIN_REGISTRATION(DocReg)
  REGDATA(progid,     "DrawPad.Drawing.17")
  REGDATA(description,"DrawPad (Step17--AutoContServer) Drawing")
  REGDATA(menuname,   "Drawing")
  REGDATA(extension,  "p17")
  REGDATA(docfilter,  "*.p17")
  REGDOCFLAGS(dtAutoOpen | dtAutoDelete | dtUpdateDir | dtCreatePrompt | dtRegisterExt)
REGDATA(debugger,   "tdw -do")
REGDATA(debugprogid,"DrawPad.Drawing.D.17")
REGDATA(debugdesc,  "DrawPad (Step17--AutoContServer, debug) Drawing")
  REGDATA(insertable, "")
  REGDATA(verb0,      "&Edit")
  REGDATA(verb1,      "&Open")
  REGFORMAT(0, DrawPadFormat,   ocrContent,  ocrHGlobal,              ocrGetSet)
  REGFORMAT(1, ocrEmbedSource,  ocrContent,  ocrIStorage,             ocrGetSet)
  REGFORMAT(2, ocrMetafilePict, ocrContent,  ocrMfPict|ocrStaticMed,  ocrGet)
  REGFORMAT(3, ocrBitmap,       ocrContent,  ocrGDI|ocrStaticMed,     ocrGet)
  REGFORMAT(4, ocrDib,          ocrContent,  ocrHGlobal|ocrStaticMed, ocrGet)
END_REGISTRATION
BEGIN_REGISTRATION(ListReg)
  REGDATA(description,"Line List")
  REGDATA(extension,  "p17")
  REGDATA(docfilter,  "*.p17")
  REGDOCFLAGS(dtAutoDelete | dtHidden)
END_REGISTRATION

DEFINE_DOC_TEMPLATE_CLASS(TDrawDocument, TDrawView,       DrawTemplate);
DEFINE_DOC_TEMPLATE_CLASS(TDrawDocument, TDrawListView,   DrawListTemplate);

DrawTemplate drawTpl(DocReg);
DrawListTemplate drawListTpl(ListReg);

//extern TDocTemplate drawTpl;

TDrawDocument*
TDrawApp::OpenDoc(const char far* name)
{
  long flags = name ? 0 : dtNewDoc;
  TDocManager* docManager = GetDocManager();
  if (!docManager)
    return 0;
  HWND hWnd = ::GetFocus();
  TDocument* doc = GetDocManager()->CreateDoc(&drawTpl, name, 0, flags);
  ::SetFocus(hWnd);
  return dynamic_cast<TDrawDocument*>(doc);
}

TDrawApp::TDrawApp() :
  TApplication(::AppReg["appname"], owl::Module, &::AppDictionary)
{
}

void
TDrawApp::InitMainWindow()
{
  View = 0;

  // Determine whether it's MDI or SDI app
  // If single use or DLL server, run multiple instances as SDI apps
  DocMode = (IsOptionSet(amSingleUse) ||
            !IsOptionSet(amExeMode)) ? dmSDI : dmMDI;

  TOleFrame* frame;
  if (DocMode == dmMDI)
    // Construct the TOleMDIFrame frame window
    frame = new TOleMDIFrame(GetName(), 0, *(Client = new TMDIClient), true);
  else
    // Construct the TOleFrame frame window
    frame = new TOleFrame(GetName(), 0, true);

  // Construct a status bar
  TStatusBar* sb = new TStatusBar(frame, TGadget::Recessed);

  // Construct a control bar
  TControlBar* cb = new TControlBar(frame);
#if OWLVersion >= 0x600
  cb->EnableFlatStyle();
#endif
  cb->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW, TButtonGadget::Command));
  cb->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN, TButtonGadget::Command));
  cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE, TButtonGadget::Command));
  cb->Insert(*new TButtonGadget(CM_FILESAVEAS, CM_FILESAVEAS, TButtonGadget::Command));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_PENSIZE, CM_PENSIZE, TButtonGadget::Command));
  cb->Insert(*new TButtonGadget(CM_PENCOLOR, CM_PENCOLOR, TButtonGadget::Command));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_PEN, CM_PEN, TButtonGadget::Exclusive));
  cb->Insert(*new TButtonGadget(CM_SELECT, CM_SELECT, TButtonGadget::Exclusive));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(CM_ABOUT, CM_ABOUT, TButtonGadget::Command));
  cb->SetHintMode(TGadgetWindow::EnterHints);

  cb->GetWindowAttr().Id = IDW_TOOLBAR;

  // Insert the status bar and control bar into the frame
  frame->Insert(*sb, TDecoratedFrame::Bottom);
  frame->Insert(*cb, TDecoratedFrame::Top);

  // Set the main window and its menu
  SetMainWindow(frame);
  if (DocMode == dmMDI)
    GetMainWindow()->SetMenuDescr(TMenuDescr(IDM_MDICMNDS));
  else
    GetMainWindow()->SetMenuDescr(TMenuDescr(IDM_SDICMNDS));

  // Install the document manager
  SetDocManager(new TDocManager(DocMode, GetApplicationObject()));
}

void
TDrawApp::InitInstance()
{
  TApplication::InitInstance();
  GetMainWindow()->DragAcceptFiles(true);
}

void
TDrawApp::EvDropFiles(TDropInfo dropInfo)
{
  int fileCount = dropInfo.DragQueryFileCount();
  for (int index = 0; index < fileCount; index++) {
    int fileLength = dropInfo.DragQueryFileNameLen(index)+1;
    char* filePath = new char [fileLength];
    dropInfo.DragQueryFile(index, filePath, fileLength);
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
    if (tpl)
      GetDocManager()->CreateDoc(tpl, filePath);
    else { // Embedding from file
      TOleWindow* oleWin = TYPESAFE_DOWNCAST(View, TOleWindow);

      // Let the TOleWindow handle the dropped file
      //
      if (oleWin)
        oleWin->ForwardMessage();
    }

    delete filePath;
  }
  dropInfo.DragFinish();
}

void
TDrawApp::EvNewView(TView& view)
{
  View = &view;
  TOleView* ov = TYPESAFE_DOWNCAST(&view, TOleView);
  if (DocMode == dmMDI) {
    if (view.GetDocument().IsEmbedded() && ov && !ov->IsOpenEditing()) {
      TWindow* vw = view.GetWindow();
      TOleFrame* mw = TYPESAFE_DOWNCAST(GetMainWindow(), TOleFrame);
      TWindow* rvb = mw->GetRemViewBucket();
      vw->SetParent(rvb);
      vw->Create();
    }
    else {
      TMDIChild* child = new TMDIChild(*Client, 0);
      if (view.GetViewMenu())
        child->SetMenuDescr(*view.GetViewMenu());
      child->Create();
      child->SetClientWindow(view.GetWindow());
    }
  }
  else {
    TFrameWindow* frame = GetMainWindow();
    frame->SetClientWindow(view.GetWindow());
    if (!view.IsOK())
      frame->SetClientWindow(0);
    else if (view.GetViewMenu())
      frame->MergeMenu(*view.GetViewMenu());
  }

  if (!ov || !ov->GetOcRemView())
    OcApp->SetOption(amEmbedding, false);
}

void
TDrawApp::EvCloseView(TView& /*view*/)
{
  // nothing needs to be done here for MDI
  if (DocMode == dmSDI) {
    GetMainWindow()->SetClientWindow(0);
  }
}

void
TDrawApp::CmAbout()
{
  TDialog(&TWindow(::GetFocus(), this), IDD_ABOUT).Execute();
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  try {
    Registrar = new TOcRegistrar(AppReg, TOleDocViewAutoFactory<TDrawApp>(),
                                 TApplication::GetCmdLine(), ::DocTemplateStaticHead);
    if (Registrar->IsOptionSet(amAnyRegOption))
      return 0;

    return Registrar->Run();
  }
  catch (const std::exception& x) {
    ::MessageBoxA(0, x.what(), "Exception", MB_OK);
  }
  return -1;
}
