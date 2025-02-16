#pragma once

#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"
#include "QuickPID.h"
#include "shared.h"
#include "manualProcessor.h"

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
  Chute &m_chute;
  Joystick &m_joystick;
  LimitSwitch &m_cwLimit;
  LimitSwitch &m_ccwLimit;
  Motor &m_motor;
  ManualProcessor m_manualProcessor;
  

  bool checkManualMode(bool);
  Mode determineMode(Mode previousMode);

public:
  Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor);
  void initialize();
  float update(bool, PIDParameters);
};