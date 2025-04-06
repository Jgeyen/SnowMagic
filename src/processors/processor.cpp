
#include "Arduino.h"
#include "processor.h"
#include "imu.h"
#include "constants.h"

Processor::Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor, SerialOutput &serialWriter)
    : m_chute(chute),
      m_joystick(joystick),
      m_cwLimit(cwLimit),
      m_ccwLimit(ccwLimit),
      m_motor(motor),
      m_serialWriter(serialWriter),
      m_manualProcessor(chute, joystick, cwLimit, ccwLimit, motor),
      m_holdPositionProcessor(chute, joystick, cwLimit, ccwLimit, motor),
      m_currentMode(Mode::Startup)
{
}

void Processor::initialize()
{
  Serial.println("Initialize processor");
  m_startingMillis = millis();
}

// int freeMemory() {
//   extern int __heap_start, *__brkval;
//   int v;
//   return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
// }

Mode Processor::determineMode()
{

  if ((m_cwLimit.isHit() && m_ccwLimit.isHit()) || !m_chute.isPositionValid())
  {
    return Mode::Error;
  }
  if (m_manualProcessor.checkIfCurrentMode())
  {
    return Mode::ManualControl;
  }
  if (millis() < m_startingMillis + 9000 || !m_chute.isPositionValid())
  {
    return Mode::Startup;
  }

  if (this->m_currentMode == Mode::ManualControl || this->m_currentMode == Mode::Startup)
  {
    return Mode::TransitionToHold;
  }
  if (m_cwLimit.isHit())
  {
    if (this->m_currentMode != Mode::ManualControl)
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

  Mode newMode = this->determineMode();

  switch (newMode)
  {
  case Mode::ManualControl:
    m_manualProcessor.update(true);
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
    // Pass the PID parameters received by Processor::update
    m_holdPositionProcessor.update(false, pidParams);
    break;
  default:
    m_holdPositionProcessor.disableHoldPosition();
    break;
  }
  if (verbose){
    // Call the refactored print method, passing object references
    m_serialWriter.printMainLoopData(
        newMode, // Pass the determined mode for this cycle
        m_motor,
        m_joystick,
        m_chute,
        m_holdPositionProcessor,
        m_cwLimit,
        m_ccwLimit
    );
  }
  this->m_currentMode = newMode;
}
