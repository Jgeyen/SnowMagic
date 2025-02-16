#pragma once
#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"
#ifdef ARDUINO
  #include <QuickPID.h>
#else
#include "../test/mocks/QuickPID.h"
#endif


class HoldPositionProcessor
{
private:
    Chute &m_chute;
    Joystick &m_joystick;
    LimitSwitch &m_cwLimit;
    LimitSwitch &m_ccwLimit;
    Motor &m_motor;

    float m_Kp = 0.02f;
    float m_Ki = 0.002f;
    float m_Kd = 0.0f;
    float m_input = 0.0f;
    double getShortestAngleDifference(double target, double current);

public:
    HoldPositionProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor);
    void update(bool verbose);
    // bool checkIfCurrentMode();
    void disableHoldPosition();
    void transitionToHold();

    float input() const;
};