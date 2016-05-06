//---------------------------------------------------------------------------

#ifndef appH
#define appH
//---------------------------------------------------------------------------
#include <owl/applicat.h>

class TMyApplication : public owl::TApplication
{
  public:
    TMyApplication();

  protected:
    virtual void InitMainWindow(); // TApplication override
};
#endif
