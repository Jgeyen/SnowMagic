#pragma once

#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"
#include "QuickPID.h"
#include "shared.h"

struct PIDParameters
{
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

class Processor
{
private:
  bool m_isManual = true;
  bool m_buttonPressLatch = false;
  int m_buttonCount;
  bool checkManualMode(bool);
  Mode determineMode(Mode, Chute &, Joystick &, LimitSwitch &, LimitSwitch &);

public:
  void initialize();
  float update(Chute &, Joystick &, LimitSwitch &, LimitSwitch &, bool, PIDParameters);
};
