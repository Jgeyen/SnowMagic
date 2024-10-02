
#include "Arduino.h";
#include "brain.h";
#include "imu.h";
#include "constants.h";

// PID Variables
//Ku = 10; Tu = 1
float Kp = 0.02;
float Ki = 0.002;
float Kd = 0;
unsigned long startingMillis;
unsigned long lastPrint = 0;
bool isStarting = true;
Mode currentMode = Mode::Startup;
float Setpoint, Input, Output;

QuickPID myPID(&Input, &Output, &Setpoint);

void Brain::Initialize(){
  startingMillis = millis();
  

  myPID.SetOutputLimits(-1, 1);
  myPID.SetSampleTimeUs(100000);
  myPID.SetTunings(Kp, Ki, Kd);
}

bool shouldPrint(){
  if(millis() < lastPrint + 500){
    Serial.println("don't print");
    return false;
  }
  lastPrint = millis();
  return true;
}
void printData(bool isManualmode, Mode currentMode, float output, float joystickPosition, float targetPosition, float currentPosition, float error, bool cwLimitHit, bool ccwLimitHit)
{
  if(!shouldPrint){
    return;
  }
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
  if(abs(difference) < 0.1){
    difference = 0;
  }

  return difference;
}

bool Brain::checkManualMode(bool buttonPressed)
{
    if (buttonPressed == true) {
        if (!m_buttonPressLatch) {
            m_buttonPressLatch = true;
            m_isManual = !m_isManual; // Update the state
            return m_isManual;
        }
    }

    if (!buttonPressed && m_buttonPressLatch) {
        m_buttonPressLatch = false;
    }
    
    // Always return the current state
    return m_isManual;
}

Mode Brain::DetermineMode(Mode previousMode, Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit)
{
  m_isManual = checkManualMode(joystick.isButtonPressed());

  if ((cwLimit.isHit() && ccwLimit.isHit()) || !ChuteIMUConnected)
  {
    return Mode::Error;
  }
  if (millis() < startingMillis + 9000)
  {
    return Mode::Startup;
  }
  if (m_isManual || joystick.isActive())
  {
    return Mode::ManualControl;
  }
  if (previousMode == Mode::ManualControl || previousMode == Mode::Startup)
  {
    return Mode::TransitionToHold;
  }
  if (cwLimit.isHit())
  {
    if (previousMode != Mode::ManualControl)
    {
      if (chute.currentPosition() + 5 < chute.targetPosition())
      {
        return Mode::TransitionToHold;
      }
      if (chute.currentPosition() + chute.totalRange() - 5 > chute.targetPosition())
      {
        return Mode::TraverseCCWToTP;
      }
    }
    return Mode::AtCWLimit;
  }
  if (ccwLimit.isHit())
  {
    return chute.currentPosition() - 5 > chute.targetPosition() ? Mode::TransitionToHold : Mode::AtCCWLimit;
  }

  return Mode::HoldPosition;
}



float Brain::Think(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, bool verbose, PIDParameters pidParams)
{

  if(Kp != pidParams.proportional || Ki != pidParams.integral || Kd != pidParams.derivative){
    Kp = pidParams.proportional;
    Ki = pidParams.integral;
    Kd = pidParams.derivative;
    Serial.println("setting PID params");
    myPID.SetTunings(Kp, Ki, Kd);
  }

  
  if(verbose && shouldPrint) {
    Serial.print("Kp:");
    Serial.print(myPID.GetKp(), 4);
    Serial.print("; Ki:");
    Serial.print(myPID.GetKi(), 4);
    Serial.print("; Kd:");
    Serial.println(myPID.GetKd(), 4);
  }

  Mode newMode = this->DetermineMode(currentMode, chute, joystick, cwLimit, ccwLimit);

  switch (newMode)
  {
  case Mode::ManualControl:
    myPID.SetMode(QuickPID::Control::manual);
    
    if (joystick.isPushedLeft() && !ccwLimit.isHit())
    {
      // Joystick moved to the left side
      Output = joystick.value();
    }
    else if (joystick.isPushedRight() && !cwLimit.isHit())
    {
      // Joystick moved to the right side
      Output = joystick.value();
    }
    else
    {
      // Joystick is in the dead zone, stop the motor
      Output = 0;
    }
    break;
  case Mode::Startup:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  case Mode::TransitionToHold:
    chute.captureTargetPosition();
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
    Input = getShortestAngleDifference(chute.targetPosition(), chute.currentPosition());

    bool computePerformed;
    computePerformed = myPID.Compute();
    if(computePerformed && Output > 0){
      Output = Output + 0.09;
    }
    if(computePerformed && Output < 0){
      Output = Output - 0.09;
    }
    break;
  default:
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
    break;
  }
  if(verbose){
    printData(m_isManual, newMode, Output, joystick.value(), chute.targetPosition(), chute.currentPosition(), Input, cwLimit.isHit(), ccwLimit.isHit());
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
