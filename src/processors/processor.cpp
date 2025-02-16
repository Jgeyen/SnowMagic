
#include "Arduino.h"
#include "processor.h"
#include "imu.h"
#include "constants.h"

// PID Variables
// Ku = 10; Tu = 1
float Kp = 0.02;
float Ki = 0.002;
float Kd = 0;
unsigned long startingMillis;
unsigned long lastPrint = 0;
bool isStarting = true;
Mode currentMode = Mode::Startup;
float Setpoint, Input, Output;

QuickPID myPID(&Input, &Output, &Setpoint);

Processor::Processor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor)
  : m_chute(chute),
    m_joystick(joystick),
    m_cwLimit(cwLimit),
    m_ccwLimit(ccwLimit),
    m_motor(motor),
    m_manualProcessor(chute, joystick, cwLimit, ccwLimit, motor)
{
}

void Processor::initialize()
{
  startingMillis = millis();

  myPID.SetOutputLimits(-1, 1);
  myPID.SetSampleTimeUs(100000);
  myPID.SetTunings(Kp, Ki, Kd);
}

// int freeMemory() {
//   extern int __heap_start, *__brkval;
//   int v;
//   return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
// }

void printData(bool isManualmode, Mode currentMode, float output, float joystickPosition, float targetPosition, float currentPosition, float error, bool cwLimitHit, bool ccwLimitHit)
{
  if (!Shared::isTimeElapsed(lastPrint,500))
  {
    return;
  }
  Serial.print("t:");
  Serial.print(static_cast<float>(millis()) / 1000);

  // Serial.print ("mem:");
  // Serial.print(freeMemory());

  Serial.print("man:");
  Serial.print(isManualmode);

  Serial.print("; out:");
  Serial.print(output);

  Serial.print("; jp:");
  Serial.print(joystickPosition);

  Serial.print("; tp:");
  Serial.print(targetPosition);

  Serial.print(";cp:");
  Serial.print(currentPosition);

  Serial.print(";er:");
  Serial.print(error);

  Serial.print(";cwl:");
  Serial.print(cwLimitHit);

  Serial.print(";ccwl:");
  Serial.print(ccwLimitHit);

  Serial.print(";cm:");
  switch (currentMode)
  {
  case Mode::ManualControl:
    Serial.println("Manual");
    break;
  case Mode::Startup:
    Serial.println("Startup");
    break;
  case Mode::TransitionToHold:
    Serial.println("Transition");
    break;
  case Mode::HoldPosition:
    Serial.println("Hold");
    break;
  case Mode::AtCWLimit:
    Serial.println("AtCWLimit");
    break;
  case Mode::AtCCWLimit:
    Serial.println("AtCCWLimit");
    break;
  case Mode::TraverseCCWToTP:
    Serial.println("TraverseCCW");
    break;
  case Mode::TraverseCWToTP:
    Serial.println("TraverseCW");
    break;
  case Mode::Error:
    Serial.println("Error");
    break;
  default:
    Serial.println("Unknown");
    break;
  }
}

double getShortestAngleDifference(double target, double current)
{
  double difference = fmod(target - current, 360.0);
  if (difference < -180.0)
  {
    difference += 360.0;
  }
  if (difference > 180.0)
  {
    difference -= 360.0;
  }
  if (abs(difference) < 0.1)
  {
    difference = 0;
  }

  return difference;
}



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

float Processor::update(bool verbose, PIDParameters pidParams)
{

  // if(Kp != pidParams.proportional || Ki != pidParams.integral || Kd != pidParams.derivative){
  //   Kp = pidParams.proportional;
  //   Ki = pidParams.integral;
  //   Kd = pidParams.derivative;
  //   Serial.println("setting PID params");
  //   myPID.SetTunings(Kp, Ki, Kd);
  // }

  // if(verbose && shouldPrint()) {
  //   Serial.print("Kp:");
  //   Serial.print(myPID.GetKp(), 4);
  //   Serial.print("; Ki:");
  //   Serial.print(myPID.GetKi(), 4);
  //   Serial.print("; Kd:");
  //   Serial.println(myPID.GetKd(), 4);
  // }

  Mode newMode = this->determineMode(currentMode);

  switch (newMode)
  {
  case Mode::ManualControl:
    
  case Mode::Startup:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  case Mode::TransitionToHold:
    m_chute.captureTargetPosition();
    myPID.SetMode(QuickPID::Control::automatic);
    break;
  case Mode::AtCWLimit:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  case Mode::AtCCWLimit:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  case Mode::HoldPosition:
    Input = getShortestAngleDifference(m_chute.targetPosition(), m_chute.currentPosition());

    bool computePerformed;
    computePerformed = myPID.Compute();
    if (computePerformed && Output > 0)
    {
      Output = Output + 0.09;
    }
    if (computePerformed && Output < 0)
    {
      Output = Output - 0.09;
    }
    break;
  default:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  }
  if (verbose)
  {
    printData(m_manualProcessor.isManual, newMode, Output, m_joystick.value(), m_chute.targetPosition(), m_chute.currentPosition(), Input, m_cwLimit.isHit(), m_ccwLimit.isHit());
  }
  currentMode = newMode;
  return Output;
}

float calcShortestYawDelta(float targetYaw, float currentYaw, bool cwDirection)
{
  float delta_yaw;

  if (cwDirection)
  {
    // Calculate clockwise delta_yaw here
  }
  else
  {
    // Calculate counter-clockwise delta_yaw here
  }

  return delta_yaw;
}

float determineDirection(float yawSetPoint, float currentYaw, LimitSwitch cwLimit, LimitSwitch ccwLimit)
{
  if (cwLimit.isHit() || ccwLimit.isHit())
  {
    // Calculate the shortest distance to the target yaw in both directions
    float delta_yaw_cw = calcShortestYawDelta(yawSetPoint, currentYaw, "clockwise");
    float delta_yaw_ccw = calcShortestYawDelta(yawSetPoint, currentYaw, "counter-clockwise");

    // If one way is shorter and we've hit the limit switch in the opposite direction
    if (abs(delta_yaw_cw) < abs(delta_yaw_ccw) && cwLimit.isHit())
    {
      // Change direction to clockwise
      yawSetPoint = currentYaw + delta_yaw_cw;
    }
    else if (abs(delta_yaw_ccw) < abs(delta_yaw_cw) && ccwLimit.isHit())
    {
      // Change direction to counter-clockwise
      yawSetPoint = currentYaw + delta_yaw_ccw;
    }
  }
}
