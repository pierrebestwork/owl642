//---------------------------------------------------------------------------

#pragma hdrstop

#include "opendlg.h"

using namespace owl;

TOpenDialog::TOpenDialog(TWindow*        parent,
                         TData&          data,
                         int             share,
                         TResId          templateId,
                         LPCTSTR         title,
                         TModule*        module)
: TFileOpenDialog(parent, data, templateId, title, module),
_share(share)
{

}

int TOpenDialog::ShareViolation()
{
  return _share;
}


