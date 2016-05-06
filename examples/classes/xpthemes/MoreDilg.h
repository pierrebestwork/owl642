//----------------------------------------------------------------------------
//  Project themes
//  
//  (C) 2007 Sebastian Ledesma
//
//  SUBSYSTEM:    themes.apx Application
//  FILE:         moredilg.h
//  AUTHOR:       Sebastian Ledesma
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TMoreDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(moredilg_h)              // Sentry, use file only if it's not already included.
#define moredilg_h

#include <owl/dialog.h>

#include "themsapp.rh"            // Definition of all resources.
#include <owl/treeviewctrl.h>
#include <owl/listviewctrl.h>
#include <owl/colmnhdr.h>
#include <owl/imagelst.h>

#include <owl/controlb.h>
#include <owl/statusba.h>
#include <owl/buttonga.h>

#include <owl/glyphbtn.h>

//{{TDialog = TMoreDialog}}
#include <pshpack1.h>
struct TMoreDialogXfer {
//{{TMoreDialogXFER_DATA}}
//{{TMoreDialogXFER_DATA_END}}
};
#include <poppack.h>

class TMoreDialog : public TDialog {
  public:
    TMoreDialog(TWindow* parent, TResId resId = IDD_MOREDIALOG, TModule* module = 0);
    virtual ~TMoreDialog();

//{{TMoreDialogXFER_DEF}}
  protected:
    TColumnHeader* pColumnHeader;
    TListViewCtrl* pListWindow;
    TTreeViewCtrl* pTreeView;

    TControlBar *pControlBar;
    TGlyphButton *pBtn_Glyph;
//{{TMoreDialogXFER_DEF_END}}
    TImageList *LgImageList;

//{{TMoreDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TMoreDialogVIRTUAL_END}}
};    //{{TMoreDialog}}


#endif  // moredilg_h sentry.

