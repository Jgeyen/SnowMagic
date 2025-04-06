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
   // PID tunings will be set in the update method based on passed parameters
   m_pid.SetMode(QuickPID::Control::manual); // Start in manual mode
}

// Update method implementation - accepts PID parameters
void HoldPositionProcessor::update(bool verbose, const PIDParameters &params)
{
    // Check if PID parameters have changed and update the controller
    // Compare passed params with current controller values
    if (params.proportional != m_pid.GetKp() ||
        params.integral != m_pid.GetKi() ||
        params.derivative != m_pid.GetKd())
    {
        m_pid.SetTunings(params.proportional, params.integral, params.derivative);
        // Optional: Add a Serial print here to confirm tuning update
        // Serial.println("PID Tunings Updated");
    }

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
    // Removed the fixed offset logic:
    // if (computePerformed && m_output > 0) { ... }
    // if (computePerformed && m_output < 0) { ... }
    // The raw PID output (m_output) will be used directly.
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