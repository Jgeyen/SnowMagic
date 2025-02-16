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

void HoldPositionProcessor::disableHoldPosition()
{
    myPID.SetMode(QuickPID::Control::manual);
    Output = 0;
}

void HoldPositionProcessor::transitionToHold()
{
    m_chute.captureTargetPosition();
    myPID.SetMode(QuickPID::Control::automatic);
}

float HoldPositionProcessor::input() const
{
    return m_input;
}

// float determineDirection(float yawSetPoint, float currentYaw, LimitSwitch cwLimit, LimitSwitch ccwLimit)
// {
//     if (cwLimit.isHit() || ccwLimit.isHit())
//     {
//         // Calculate the shortest distance to the target yaw in both directions
//         float delta_yaw_cw = calcShortestYawDelta(yawSetPoint, currentYaw, "clockwise");
//         float delta_yaw_ccw = calcShortestYawDelta(yawSetPoint, currentYaw, "counter-clockwise");

//         // If one way is shorter and we've hit the limit switch in the opposite direction
//         if (abs(delta_yaw_cw) < abs(delta_yaw_ccw) && cwLimit.isHit())
//         {
//             // Change direction to clockwise
//             yawSetPoint = currentYaw + delta_yaw_cw;
//         }
//         else if (abs(delta_yaw_ccw) < abs(delta_yaw_cw) && ccwLimit.isHit())
//         {
//             // Change direction to counter-clockwise
//             yawSetPoint = currentYaw + delta_yaw_ccw;
//         }
//     }
// }

// float calcShortestYawDelta(float targetYaw, float currentYaw, bool cwDirection)
// {
//     float delta_yaw;

//     if (cwDirection)
//     {
//         // Calculate clockwise delta_yaw here
//     }
//     else
//     {
//         // Calculate counter-clockwise delta_yaw here
//     }

//     return delta_yaw;
// }