#pragma once

#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"
#include "serialOutput.h"
#include "shared.h"
#include "manualProcessor.h"
#include "holdPositionProcessor.h"
#include "pidParameters.h"

class Processor
{
private:
  Chute &m_chute;
  Joystick &m_joystick;
  LimitSwitch &m_cwLimit;
  LimitSwitch &m_ccwLimit;
  Motor &m_motor;
  ManualProcessor m_manualProcessor;
  HoldPositionProcessor m_holdPositionProcessor;
  SerialOutput m_serialWriter;
  unsigned long startingMillis;

  Mode determineMode(Mode previousMode);

public:
  Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor, SerialOutput &serialWriter);
  void initialize();
  void update(bool, PIDParameters);
};