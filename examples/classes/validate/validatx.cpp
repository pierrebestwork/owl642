//
// Validate Example - edit field validation using TValidator classes
//

#include <owl/applicat.h>
#include <owl/dialog.h>
#include <owl/framewin.h>
#include <owl/edit.h>
#include <owl/checkbox.h>
#include <owl/validate.h>
#include <owl/transfer.h>
#include <vector>

#include "validate.rc"               

//
// Employee details; used for dialog data transfer.
//
struct TEmployeeInformation
{
  owl::tstring Name;
  owl::tstring SocialSecurityNumber;
  owl::tstring EmployeeId;
  owl::tstring Department;
  owl::tstring SecurityClearance;
  bool FullTime;
  bool Permanent;
  bool Exempt;
  owl::tstring CustomData;
};

// 
// Use the dialog data transfer (DDT) class template TTransferDialog as our base dialog.
// This template is new in OWLNext 6.32.
//
typedef owl::TTransferDialog<TEmployeeInformation> TEmployeeBaseDlg;

//
// Employee information entry dialog
//
class TEmployeeDlg 
  : public TEmployeeBaseDlg 
{
public:

  TEmployeeDlg(owl::TWindow* parent, TEmployeeInformation&);

protected:

  virtual void DoTransferData(const owl::TTransferInfo&, TEmployeeInformation&); // override

private:

  owl::TEdit& CustomData;

  void CmSetCustom();

  DECLARE_RESPONSE_TABLE(TEmployeeDlg);
};

//
// Our sample application
// Sets up a simple frame window with a menu containing the test command that brings up the
// Employee Information test dialog.
// 
class TValidateExample 
  : public owl::TApplication 
{
public:

  TValidateExample() 
    : owl::TApplication(_T("Validate Example"))
  {}

protected:

  virtual void InitMainWindow(); // override

private:

  void CmEmployeeInput();

  DECLARE_RESPONSE_TABLE(TValidateExample);
};

//-------------------------------------------------------------------------------------------------

//
// Allow ourselves to be a little lax about namespace qualification.
// Never do this in a header though!
//
using namespace owl;

//
// Event handler bindings
//
DEFINE_RESPONSE_TABLE1(TEmployeeDlg, TDialog)
  EV_COMMAND(IDC_CUSTOM, CmSetCustom),
END_RESPONSE_TABLE;

//
// Constructor; sets up the validators we want to associate with the fields.
//
TEmployeeDlg::TEmployeeDlg(TWindow* parent, TEmployeeInformation& dataSource)
  : TEmployeeBaseDlg(parent, IDD_EMPLOYEEINFO, dataSource),
  CustomData(*new TEdit(this, IDC_EDIT2))
{
  // Use a filter validator for the name.
  // Here we limit allowed characters to letters, space and punctuation.
  // 
  (new TEdit(this, IDC_NAME))
    ->SetValidator(new TFilterValidator(_T("A-Za-z. ")));

  // Use a picture validator for the Social Security number.
  // Here we specify the valid form of the number, including conventional punctuation.
  //
  (new TEdit(this, IDC_SS))
    ->SetValidator(new TPXPictureValidator(_T("###-##-####")));

  // Use a range validator for the employee number.
  // Here we limit the number to the range [1, 999].
  //
  (new TEdit(this, IDC_EMPID))
    ->SetValidator(new TRangeValidator(1, 999));

  // Use a picture validator for the department field.
  // Here we specify a set of allowed values.
  //
  (new TEdit(this, IDC_DEPT))
    ->SetValidator(new TPXPictureValidator(_T("Sales,Dev,Mfg")));

  // Use a picture validator for the security clearance code.
  // Here we limit the field to a set of allowed values.
  //
  (new TEdit(this, IDC_SECURITY))
    ->SetValidator(new TPXPictureValidator(_T("11,12,13,14,15")));

  // Use a picture validator for the custom input field.
  // This demonstrates dynamic application of custom pictures (input format templates).
  // See CmSetCustom.
  //
  TValidator* v = new TPXPictureValidator(_T("###"), true);
  v->UnsetOption(voOnAppend);
  CustomData.SetValidator(v);
}

//
// Dialog data transfer (using the new DDT framework introduced in OWLNext 6.32)
// For more information about the DDT framework, see the OWLNext wiki:
// http://sourceforge.net/apps/mediawiki/owlnext/index.php?title=Dialog_Data_Transfer
//
void TEmployeeDlg::DoTransferData(const owl::TTransferInfo& i, TEmployeeInformation& e)
{
  TransferEditData(i, IDC_NAME, e.Name);
  TransferEditData(i, IDC_SS, e.SocialSecurityNumber);
  TransferEditData(i, IDC_EMPID, e.EmployeeId);
  TransferEditData(i, IDC_DEPT, e.Department);
  TransferEditData(i, IDC_SECURITY, e.SecurityClearance);
  TransferCheckBoxData(i, IDC_FTIME, e.FullTime);
  TransferCheckBoxData(i, IDC_PERMANENT, e.Permanent);
  TransferCheckBoxData(i, IDC_EXEMPT, e.Exempt);
  tstring picture = _T("###");
  TransferEditData(i, IDC_EDIT1, picture);
  TransferEditData(i, CustomData, e.CustomData);
}

//
// Update the picture (input format template) for the custom input field.
// Loads the picture from the picture field and applies it to the validator for the
// custom input field.
//
void TEmployeeDlg::CmSetCustom()
{
  try 
  {
    // Get the user-defined picture (input format template).
    //
    tstring picture = GetDlgItemText(IDC_EDIT1);

    // Apply this user-defined picture to the custom input field.
    // Note that an edit field owns its validator, so the old validator is automatically 
    // deallocated when we apply the new validator via SetValidator.
    //
    TValidator* v = new TPXPictureValidator(picture, true);
    v->UnsetOption(voOnAppend);
    CustomData.SetValidator(v);

    // Move focus to the custom input field, ready for testing the new picture.
    //
    CustomData.SetFocus();
  }
  catch (const TXValidator& x) 
  {                    
    // Catches any syntax errors in the specified picture.
    //
    MessageBox(x.why(), GetApplication()->GetName());
    return;
  }
}

//
// Application window setup
//
void TValidateExample::InitMainWindow()
{
  TFrameWindow* f = new TFrameWindow(0, GetName());
  f->AssignMenu(IDM_MAINMENU);
  f->SetWindowPos(0, 0, 0, 400, 200, SWP_NOMOVE);
  SetMainWindow(f);
}

//
// Handler for the test command
// Displays the Employee Information test dialog.
//
void TValidateExample::CmEmployeeInput()
{
  static TEmployeeInformation data =
  {
    _T("Frank Foo"), // Name
    _T("123-45-6789"), // SocialSecurityNumber
    _T("123"), // EmployeeId
    _T("Sales"), // Department
    _T("15"), // SecurityClearance
    true, // FullTime
    true, // Permanent
    true, // Exempt
    _T("999"), // CustomData
  };
  TEmployeeDlg dlg(GetMainWindow(), data);
  if (dlg.Execute() != IDOK) return;

  tostringstream report;	  
  report << _T("Name ") << data.Name << _T('\n')
    << _T("SS#: ") << data.SocialSecurityNumber << _T('\n')
    << _T("Emp. ID: ") << data.EmployeeId << _T('\n')
    << _T("Dept: ") << data.Department << _T('\n')
    << _T("Security: ") << data.SecurityClearance << _T('\n')
    << _T("State: ") << (data.FullTime ? _T("FullTime ") : _T("PartTime "))
    << (data.Permanent ? _T("Permanent ") : _T("Temporary "))
    << (data.Exempt ? _T("Exempt ") : _T("NonExempt ")) << _T('\n')
    << _T("Custom data: ") << data.CustomData;
  GetMainWindow()->MessageBox(report.str(), _T("Employee Information"));
}

DEFINE_RESPONSE_TABLE1(TValidateExample, TApplication)
  EV_COMMAND(CM_EMPINPUT, CmEmployeeInput),
END_RESPONSE_TABLE;

int OwlMain(int, LPTSTR []) // argc, argv
{
  return TValidateExample().Run();
}
