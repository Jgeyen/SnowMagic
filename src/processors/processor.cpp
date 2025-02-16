
#include "Arduino.h"
#include "processor.h"
#include "imu.h"
#include "constants.h"

Mode currentMode = Mode::ManualControl;

Processor::Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor, SerialOutput &serialWriter)
    : m_chute(chute),
      m_joystick(joystick),
      m_cwLimit(cwLimit),
      m_ccwLimit(ccwLimit),
      m_motor(motor),
      m_serialWriter(serialWriter),
      m_manualProcessor(chute, joystick, cwLimit, ccwLimit, motor),
      m_holdPositionProcessor(chute, joystick, cwLimit, ccwLimit, motor)
{
}

void Processor::initialize()
{
  startingMillis = millis();
}

// int freeMemory() {
//   extern int __heap_start, *__brkval;
//   int v;
//   return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
// }

Mode Processor::determineMode(Mode previousMode)
{

  if ((m_cwLimit.isHit() && m_ccwLimit.isHit()) || !m_chute.isPositionValid())
  {
    return Mode::Error;
  }
  if (m_manualProcessor.checkIfCurrentMode())
  {
    return Mode::ManualControl;
  }
  if (millis() < startingMillis + 9000 || !m_chute.isPositionValid())
  {
    return Mode::Startup;
  }

  if (previousMode == Mode::ManualControl || previousMode == Mode::Startup)
  {
    return Mode::TransitionToHold;
  }
  if (m_cwLimit.isHit())
  {
    if (previousMode != Mode::ManualControl)
    {
      if (m_chute.currentPosition() + 5 < m_chute.targetPosition())
      {
        return Mode::TransitionToHold;
      }
      if (m_chute.currentPosition() + m_chute.totalRange() - 5 > m_chute.targetPosition())
      {
        return Mode::TraverseCCWToTP;
      }
    }
    return Mode::AtCWLimit;
  }
  if (m_ccwLimit.isHit())
  {
    return m_chute.currentPosition() - 5 > m_chute.targetPosition() ? Mode::TransitionToHold : Mode::AtCCWLimit;
  }

  return Mode::HoldPosition;
}

void Processor::update(bool verbose, PIDParameters pidParams)
{
  m_chute.update();

  Mode newMode = this->determineMode(currentMode);

  switch (newMode)
  {
  case Mode::ManualControl:

  case Mode::Startup:
    m_holdPositionProcessor.disableHoldPosition();
    break;
  case Mode::TransitionToHold:
    m_holdPositionProcessor.transitionToHold();
    break;
  case Mode::AtCWLimit:
    m_holdPositionProcessor.disableHoldPosition();
    break;
  case Mode::AtCCWLimit:
    m_holdPositionProcessor.disableHoldPosition();
    break;
  case Mode::HoldPosition:
    m_holdPositionProcessor.update(false);
    break;
  default:
    m_holdPositionProcessor.disableHoldPosition();
    break;
  }
  if (verbose)
  {
    m_serialWriter.printMainLoopData(m_manualProcessor.isManual, newMode, m_motor.speed(), m_joystick.value(), m_chute.targetPosition(), m_chute.currentPosition(), m_holdPositionProcessor.input(), m_cwLimit.isHit(), m_ccwLimit.isHit());
  }
  currentMode = newMode;
}
