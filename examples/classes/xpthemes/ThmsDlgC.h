//----------------------------------------------------------------------------
//  Project Themes
//  
//  (C) 2007 Sebastian Ledesma
//
//  SUBSYSTEM:    Themes Application
//  FILE:         thmsdlgc.h
//  AUTHOR:       Sebastian Ledesma
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TThemesDlgClient (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(thmsdlgc_h)              // Sentry, use file only if it's not already included.
#define thmsdlgc_h

#include "themsapp.rh"                  // Definition of all resources.


#include <owl/commctrl.h>

#include <owl/tabctrl.h>
#include <owl/gauge.h>
#include <owl/progressbar.h>
#include <owl/listbox.h>
#include <owl/combobox.h>

#include <pshpack1.h>
struct TThemesDlgClientXfer {
    TComboBoxData  pComboBox1;
    TListBoxData  pListBox1;
};
#include <poppack.h>

class TThemesDlgClient : public TDialog {
  public:
    TThemesDlgClient(TWindow* parent, TResId resId = IDD_CLIENT, TModule* module = 0);
    virtual ~TThemesDlgClient();


  public:
    virtual void SetupWindow();
    virtual void CleanupWindow();

  protected:
    void EvTimer(uint timerId);
    void OnButton1();
DECLARE_RESPONSE_TABLE(TThemesDlgClient);

  protected:
    TComboBox* pComboBox1;
    TListBox* pListBox1;
    TGauge* pGauge;
    TProgressBar* pProgress;
    TTabControl* pTabControl;

};


#endif  // thmsdlgc_h sentry.
