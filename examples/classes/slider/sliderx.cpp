//
// TSlider Example - Home Heater Simulator
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved

#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/slider.h>
#include <owl/gauge.h>
#include <owl/static.h>

using namespace owl;

class TMainWindow 
  : public TWindow 
{
public:

  TMainWindow();

protected:

  virtual void SetupWindow(); // override
  void EvTimer(uint timerId);

private:

  enum
  {
    IDC_THERMOSTAT = 201,
    IDC_HEATERTIME,
    IDC_OUTSIDETEMP,
    IDC_STATICTEMP,
    IDC_STATICTIME,
    IDC_STATICOTEMP,
    IDC_THERMOMETER,
  };

  enum {IDT_REFRESH = 1};

  double Temp;
  bool IsHeaterOn;
  TStatic TempStatic;
  TGauge ThermometerGauge;
  THSlider ThermostatSlider;
  TStatic HysteresisStatic;
  TVSlider HysteresisSlider;
  TStatic OutsideTempStatic;
  TVSlider OutsideTempSlider;

  void UpdateTemp();
  void UpdateHysteresis(uint = 0);
  void UpdateOutsideTemp(uint = 0);

  void SimulateHeater();

  DECLARE_RESPONSE_TABLE(TMainWindow);
};

DEFINE_RESPONSE_TABLE1(TMainWindow, TWindow)
  EV_WM_TIMER,
  EV_CHILD_NOTIFY_ALL_CODES(IDC_HEATERTIME, UpdateHysteresis),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_OUTSIDETEMP, UpdateOutsideTemp),
END_RESPONSE_TABLE;

TMainWindow::TMainWindow()
  : TWindow(0, 0, 0), 
  Temp(40.0), 
  IsHeaterOn(false),
  TempStatic(this, IDC_STATICTEMP, "", 110, 30, 160, 17, 0),
  ThermometerGauge(this, "%d\xB0", IDC_THERMOMETER, 70, 70, 240, 24, true, 2),
  ThermostatSlider(this, IDC_THERMOSTAT, 70, 110, 240, 40),
  HysteresisStatic(this, IDC_STATICTIME, "", 4, 10, 160, 17, 0),
  HysteresisSlider(this, IDC_HEATERTIME, 20, 30, 32, 160),
  OutsideTempStatic(this, IDC_STATICOTEMP, "", 216, 10, 160, 17, 0),
  OutsideTempSlider(this, IDC_OUTSIDETEMP, 330, 30, 32, 160)
{
  MoveWindow(0, 0, 380, 200);
  SetBkgndColor(TColor::Sys3dFace);

  TempStatic.ModifyStyle(0, SS_CENTER);
  ThermostatSlider.ModifyStyle(0, TBS_AUTOTICKS);
  HysteresisStatic.ModifyStyle(0, SS_LEFT);
  HysteresisSlider.ModifyStyle(0, TBS_AUTOTICKS);
  OutsideTempStatic.ModifyStyle(0, SS_RIGHT);
  OutsideTempSlider.ModifyStyle(0, TBS_AUTOTICKS);
}

void
TMainWindow::SetupWindow()
{
  TWindow::SetupWindow();

  ThermometerGauge.SetRange(40, 120);
  ThermometerGauge.SetValue(80);

  ThermostatSlider.SetRange(40, 120);
  ThermostatSlider.SetRuler(10, false);
  ThermostatSlider.SetPosition(80);

  HysteresisSlider.SetRange(0, 10);
  HysteresisSlider.SetRuler(5, false);
  HysteresisSlider.SetPosition(5);

  OutsideTempSlider.SetRange(20, 90);
  OutsideTempSlider.SetRuler(10, false);
  OutsideTempSlider.SetPosition(40);

  SetTimer(IDT_REFRESH, 1000);

  UpdateTemp();
  UpdateHysteresis();
  UpdateOutsideTemp();
}

void
TMainWindow::EvTimer(uint /*timerId*/)
{
  SimulateHeater();
  UpdateTemp();
}

void
TMainWindow::UpdateTemp()
{
  std::ostringstream s;
  s << "Heater is " << (IsHeaterOn ? "on" : "off");
  TempStatic.SetText(s.str());
  ThermometerGauge.SetValue(static_cast<int>(Temp + 0.5));
}

void
TMainWindow::UpdateHysteresis(uint)
{
  std::ostringstream s;
  s << HysteresisSlider.GetPosition() << "\xB0 hysteresis";
  HysteresisStatic.SetText(s.str());
}

void
TMainWindow::UpdateOutsideTemp(uint)
{
  std::ostringstream s;
  s << OutsideTempSlider.GetPosition() << "\xB0 outside";
  OutsideTempStatic.SetText(s.str());
}

void
TMainWindow::SimulateHeater()
{
  Temp += (IsHeaterOn ? 2.0 : 0.0) + // heater output
    (OutsideTempSlider.GetPosition() - Temp) / 40.0; // heat transfer

  int tempSetting = ThermostatSlider.GetPosition(); 
  int hysteresis = HysteresisSlider.GetPosition();
  double lowTriggerTemp = tempSetting - hysteresis;
  double highTriggerTemp = tempSetting + hysteresis;

  if (!IsHeaterOn && Temp <= lowTriggerTemp)
    IsHeaterOn = true;
  else if (IsHeaterOn && Temp >= highTriggerTemp)
    IsHeaterOn = false;
}

class THomeHeaterSimulator 
  : public TApplication 
{
public:

  THomeHeaterSimulator()
    : TApplication("Home Heater Simulator")
  {}

protected:

  virtual void InitMainWindow() // override
  {
    TFrameWindow* frame = new TFrameWindow(0, GetName(), new TMainWindow, true);
    frame->ModifyStyle(WS_SIZEBOX | WS_MAXIMIZEBOX, 0);
    frame->EnableKBHandler();
    SetMainWindow(frame);
  }
};

int
OwlMain(int, char* []) // argc, argv
{
  THomeHeaterSimulator app;
  return app.Run();
}
