#include "manualProcessor.h"

ManualProcessor::ManualProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor)
    : m_chute(chute),
      m_joystick(joystick),
      m_cwLimit(cwLimit),
      m_ccwLimit(ccwLimit),
      m_motor(motor)
{
}

bool ManualProcessor::userSelectedManualMode()
{
    // TODO: remove this
    isManual = true;
    return isManual;

    bool buttonPressed = m_joystick.isButtonPressed();
    if (buttonPressed == true)
    {
        if (!m_buttonPressLatch)
        {
            m_buttonPressLatch = true;
            isManual = !isManual; // Update the state
            return isManual;
        }
    }

    if (!buttonPressed && m_buttonPressLatch)
    {
        m_buttonPressLatch = false;
    }

    // Always return the current state
    return isManual;
}

void ManualProcessor::update(bool verbose)
{
    float motorSpeed = 0;
    // myPID.SetMode(QuickPID::Control::manual);

    if (m_joystick.isPushedLeft() && !m_ccwLimit.isHit())
    {
        // Joystick moved to the left side
        motorSpeed = m_joystick.value();
    }
    else if (m_joystick.isPushedRight() && !m_cwLimit.isHit())
    {
        // Joystick moved to the right side
        motorSpeed = m_joystick.value();
    }
    else
    {
        // Joystick is in the dead zone, stop the motor
        motorSpeed = 0;
    }
    m_motor.setMotorSpeed(motorSpeed);
}

bool ManualProcessor::checkIfCurrentMode()
{
    userSelectedManualMode();
    return isManual || m_joystick.isActive();
}