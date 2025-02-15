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
  Chute &m_chute;
  Joystick &m_joystick;
  LimitSwitch &m_cwLimit;
  LimitSwitch &m_ccwLimit;
  bool checkManualMode(bool);
  Mode determineMode(Mode previousMode);

public:
  Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit);
  void initialize();
  float update(bool, PIDParameters);
};
