#pragma once

#include "limitSwitch.h";
#include "chute.h";
#include "joystick.h";
#include "motor.h";
#include "QuickPID.h";

struct PIDParameters {
  float proportional;
  float integral;
  float derivative;
};

enum class Mode
{
  Startup,
  ManualControl,
  TransitionToHold,
  HoldPosition,
  AtCWLimit,
  AtCCWLimit,
  TraverseCWToTP,
  TraverseCCWToTP,
  Error
};

class Brain
{
private:
  bool m_isManual = true;
  bool m_buttonPressLatch = false;
  int m_buttonCount;
  bool checkManualMode(bool);
  Mode DetermineMode(Mode, Chute &, Joystick &, LimitSwitch &, LimitSwitch &);

public:
  void Initialize() ;
  float Think(Chute &, Joystick &, LimitSwitch &, LimitSwitch &, bool, PIDParameters);


};

