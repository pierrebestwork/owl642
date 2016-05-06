//---------------------------------------------------------------------------

#ifndef frameH
#define frameH
//---------------------------------------------------------------------------

#include <owl/framewin.h>

class TMyFrame : public owl::TFrameWindow
{
  public:
    TMyFrame(owl::TWindow* parent, LPCTSTR title);
    virtual ~TMyFrame();

  protected:
    virtual void SetupWindow(); // TWindow override
    virtual void CleanupWindow(); // TWindow override

    void CmAbout();

    void CmFileOpen();

    void CmShareWarn();
    void CmShareNoWarn();
    void CmShareFallThrough();

    void CmExplorer();

    void CeShareWarn(owl::TCommandEnabler& ce);
    void CeShareNoWarn(owl::TCommandEnabler& ce);
    void CeShareFallThrough(owl::TCommandEnabler& ce);

    void CeExplorer(owl::TCommandEnabler& ce);

  private:
    void ShowMessage(const owl::tstring& message);

    int share;
    bool explorer;

  DECLARE_RESPONSE_TABLE(TMyFrame);
};

#endif
