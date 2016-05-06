//
// This test program exercises the exception transport mechanism in OWLNext.
//
// 32-bit OWL 5 and OWLNext pre-6.33 relied on Windows Structured Exception Handling (SEH) to 
// transport unhandled exceptions across the Window API boundary between handler and message loop.
// This was non-standard, compiler-dependent and proved to be unreliable (see e.g. [bugs:#230]).
// Since 6.33, unhandled exceptions in event handlers are now suspended and rethrown in the message 
// loop, similar to the 16-bit implementation of OWL 5.
//
// See TWindow::ReceiveMessage, TApplication::SuspendThrow, ResumeThrow and MessageLoop.
//
// Copyright (c) 2012 Vidar Hasfjord
// Distributed under the OWLNext License (see http://owlnext.sourceforge.net).
//

#include <owl/dialog.h>
#include <owl/propsht.h>
#include <owl/applicat.h>
#include <owl/framewin.h>

#include "resource.h"

using namespace owl;

class TExceptionalBase
  : public virtual TWindow
{
public:

  TExceptionalBase(TWindow* parent)
    : TWindow(parent)
  {}

protected:

  void CmTXBase() {throw TXBase(_T("TXBase"));}
  void CmStdException() {throw std::runtime_error("std::runtime_error");}
  void CmIntException() {throw 42;}
  void CmDiagException() {PRECONDITIONX(false, _T("Assertion violation!"));}

  //
  // Aborts the program by posting a WM_ENDSESSION message.
  //
  // Before OWLNext 6.32.5, TWindow::EvEndSession would generate a TXEndSession exception. Now 
  // TWindow::EvEndSession does nothing (only logs a message that the message was not handled), and 
  // the WM_ENDSESSION message has special handling in the message loop, causing the TXEndSession 
  // exception to be thrown there (thus needing no exception transport). See [bugs:#231].
  //
  void CmEndSession() {PostMessage(WM_ENDSESSION, TRUE);}

  DECLARE_RESPONSE_TABLE(TExceptionalBase);
};

DEFINE_RESPONSE_TABLE1(TExceptionalBase, TWindow)
  EV_COMMAND(IDC_TXBASE, CmTXBase),
  EV_COMMAND(IDC_STDEXCEPTION, CmStdException),
  EV_COMMAND(IDC_INTEXCEPTION, CmIntException),
  EV_COMMAND(IDC_DIAGEXCEPTION, CmDiagException),
  EV_COMMAND(IDC_ENDSESSION, CmEndSession),
END_RESPONSE_TABLE;

class TExceptionalDialog
  : public TExceptionalBase,
  public TDialog
{
public:

  TExceptionalDialog(TWindow* parent)
    : TExceptionalBase(parent),
    TDialog(parent, IDD_EXCEPTIONAL_DIALOG)
  {}

  DECLARE_RESPONSE_TABLE(TExceptionalDialog);
};

DEFINE_RESPONSE_TABLE2(TExceptionalDialog, TExceptionalBase, TDialog)
END_RESPONSE_TABLE;

class TExceptionalPage
  : public TExceptionalBase,
  public TPropertyPage
{
public:

  TExceptionalPage(TPropertySheet* parent)
    : TExceptionalBase(parent), 
    TPropertyPage(parent, IDD_EXCEPTIONAL_PAGE, _T("Exceptional page"))
  {}

  DECLARE_RESPONSE_TABLE(TExceptionalPage);
};

DEFINE_RESPONSE_TABLE2(TExceptionalPage, TExceptionalBase, TPropertyPage)
END_RESPONSE_TABLE;

class TExceptionalClient
  : public TExceptionalBase,
  public TDialog
{
public:

  TExceptionalClient(TWindow* parent)
    : TExceptionalBase(parent),
    TDialog(parent, IDD_EXCEPTIONAL_CLIENT)
  {}

protected:

  virtual void SetupWindow() // override
  {
    TDialog::SetupWindow();
    CheckDlgButton(IDC_MODAL, BST_CHECKED);
  }

  void CmDialog() 
  {
    bool shouldOpenModally = IsDlgButtonChecked(IDC_MODAL) == BST_CHECKED;
    if (shouldOpenModally)
    {
      TExceptionalDialog d(this);
      d.Execute();
    }
    else
    {
      TExceptionalDialog* d = new TExceptionalDialog(this);
      d->Create();
      d->ShowWindow(SW_SHOW);
    }
  }

  void CmPropertySheet()
  {
    tstring title = _T("Exceptional property sheet");
    bool shouldOpenModally = IsDlgButtonChecked(IDC_MODAL) == BST_CHECKED;
    if (shouldOpenModally)
    {
      TPropertySheet s(this, title);
      TExceptionalPage p(&s);
      s.Run(true);
    }
    else
    {
      TPropertySheet* s = new TPropertySheet(this, title);
      new TExceptionalPage(s);
      s->Run(false);
    }
  }

  DECLARE_RESPONSE_TABLE(TExceptionalClient);
};

DEFINE_RESPONSE_TABLE2(TExceptionalClient, TExceptionalBase, TDialog)
  EV_COMMAND(IDC_DIALOG, CmDialog),
  EV_COMMAND(IDC_PROPERTYSHEET, CmPropertySheet),
END_RESPONSE_TABLE;

class TExceptionTransportTest
  : public TApplication
{
public:

  TExceptionTransportTest() 
    : TApplication(_T("OWLNext Exception Transport Test")) 
  {}

protected:

  virtual void InitMainWindow() // override
  {
    TFrameWindow* f = new TFrameWindow(0, GetName(), new TExceptionalClient(0), true);
    f->ModifyStyle(WS_SIZEBOX | WS_MAXIMIZEBOX, 0);
    SetMainWindow(f);
  }

};

//
// Runs the test and logs unhandled exceptions.
//
int OwlMain(int, LPTSTR []) // argc, argv
{
  try
  {
    TExceptionTransportTest test;
    return test.Run();
  }
  catch (const std::exception& x)
  {
    TRACE(_T("Exiting due to unhandled std::exception: ") << x.what()); InUse(x);
    throw;
  }
  catch (const TXEndSession&)
  {
    TRACE(_T("Exiting due to WM_ENDSESSION."));
    throw;
  }
  catch (int x)
  {
    TRACE(_T("Exiting due to unhandled exception of type 'int': ") << x); InUse(x);
    throw;
  }
  catch (...)
  {
    TRACE(_T("Exiting due to unknown unhandled exception."));
    throw;
  }
}
