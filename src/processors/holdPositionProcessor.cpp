#include "holdPositionProcessor.h"

HoldPositionProcessor::HoldPositionProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor)
    : m_chute(chute),
      m_joystick(joystick),
      m_cwLimit(cwLimit),
      m_ccwLimit(ccwLimit),
      m_motor(motor),
      m_pid(&m_input, &m_output, &m_setpoint)
{
   // Initialize PID settings
   m_setpoint = 0.0f; 
   m_input = 0.0f;
   m_output = 0.0f;
   m_pid.SetOutputLimits(-1, 1);
   m_pid.SetSampleTimeUs(100000);
   // TODO: Set tunings based on PIDParameters struct passed from Processor
   // Using default (or potentially zero) tunings for now
   m_pid.SetTunings(0.02f, 0.002f, 0.0f); // Placeholder - use passed params later
   m_pid.SetMode(QuickPID::Control::manual); // Start in manual mode
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

    // Calculate error and store in member m_input
    m_input = getShortestAngleDifference(m_chute.targetPosition(), m_chute.currentPosition());

    bool computePerformed;
    computePerformed = m_pid.Compute(); // Use member PID controller
    // Use member m_output
    if (computePerformed && m_output > 0)
    {
        m_output = m_output + 0.09; // Apply offset if needed (consider removing/tuning)
    }
    if (computePerformed && m_output < 0)
    {
        m_output = m_output - 0.09; // Apply offset if needed (consider removing/tuning)
    }
    m_motor.setMotorSpeed(m_output); // Use member PID output
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
    m_pid.SetMode(QuickPID::Control::manual);
    m_output = 0;
}

void HoldPositionProcessor::transitionToHold()
{
    m_chute.captureTargetPosition();
    m_pid.SetMode(QuickPID::Control::automatic);
}

float HoldPositionProcessor::input() const
{
    return m_input;
}

// --- Getters ---

float HoldPositionProcessor::getOutput() const {
    return m_output;
}

float HoldPositionProcessor::getKp() {
    return m_pid.GetKp();
}

float HoldPositionProcessor::getKi() {
    return m_pid.GetKi();
}

float HoldPositionProcessor::getKd() {
    return m_pid.GetKd();
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