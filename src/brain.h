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

class Brain
{
private:
  bool m_isManual;
  bool m_buttonPressLatch = false;
  int m_buttonCount;
  bool checkManualMode(bool);

public:
  void Initialize() ;
  float Think(Chute &, Joystick &, LimitSwitch &, LimitSwitch &, bool, PIDParameters);


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