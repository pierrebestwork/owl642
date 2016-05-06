//----------------------------------------------------------------------------
//  Project Themes
//  
//  (C) 2007 Sebastian Ledesma
//
//  SUBSYSTEM:    Themes Application
//  FILE:         themsapp.h
//  AUTHOR:       Sebastian Ledesma
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TThemesApp (TApplication).
//
//----------------------------------------------------------------------------
#if !defined(themsapp_h)              // Sentry, use file only if it's not already included.
#define themsapp_h

#include <owl/controlb.h>
#include <owl/docking.h>
#include <owl/opensave.h>


#include "themsapp.rh"            // Definition of all resources.


//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = TSDIDecFrame}}
class TSDIDecFrame : public TDecoratedFrame {
  public:
    TSDIDecFrame(TWindow* parent, LPCTSTR title, TWindow* clientWnd, bool trackMenuSelection = false, TModule* module = 0);
    ~TSDIDecFrame();

//{{TThemesAppVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TThemesAppVIRTUAL_END}}
};    //{{TSDIDecFrame}}


//{{TApplication = TThemesApp}}
class TThemesApp : public TApplication {
  private:


  public:
    TThemesApp();
    virtual ~TThemesApp();

    THarbor*        ApxHarbor;

//{{TThemesAppVIRTUAL_BEGIN}}
  public:
    virtual void InitMainWindow();
//{{TThemesAppVIRTUAL_END}}

//{{TThemesAppRSP_TBL_BEGIN}}
  protected:
    void CmHelpAbout();
//{{TThemesAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TThemesApp);
};    //{{TThemesApp}}


#endif  // themsapp_h sentry.
