#include "holdPositionProcessor.h"

float Setpoint, Input, Output;
QuickPID myPID(&Input, &Output, &Setpoint);

HoldPositionProcessor::HoldPositionProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor)
    : m_chute(chute),
      m_joystick(joystick),
      m_cwLimit(cwLimit),
      m_ccwLimit(ccwLimit),
      m_motor(motor)
{
    myPID.SetOutputLimits(-1, 1);
    myPID.SetSampleTimeUs(100000);
    myPID.SetTunings(m_Kp, m_Ki, m_Kd);
}



void HoldPositionProcessor::update(bool verbose)
{
    Input = getShortestAngleDifference(m_chute.targetPosition(), m_chute.currentPosition());
    m_input = Input;

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
    m_motor.setMotorSpeed(Output);
}
double HoldPositionProcessor::getShortestAngleDifference(double target, double current)
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

void HoldPositionProcessor::disableHoldPosition(){
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
}

void HoldPositionProcessor::transitionToHold(){
    m_chute.captureTargetPosition();
    myPID.SetMode(QuickPID::Control::automatic);
}

float HoldPositionProcessor::input() const {
    return m_input;
}