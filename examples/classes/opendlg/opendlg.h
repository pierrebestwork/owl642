//---------------------------------------------------------------------------

#ifndef opendlgH
#define opendlgH
//---------------------------------------------------------------------------
#include <owl/opensave.h>

class TOpenDialog : public owl::TFileOpenDialog
{
  private:
   int _share;

  protected:
    virtual int ShareViolation();

  public:
    TOpenDialog(owl::TWindow*                parent,
                owl::TFileOpenDialog::TData& data,
                int                          share,
                owl::TResId                  templateId = 0,
                LPCTSTR                      title = 0,
                owl::TModule*                module = 0);
};


#endif
