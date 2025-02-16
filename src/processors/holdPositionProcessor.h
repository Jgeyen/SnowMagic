#pragma once
#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"
#include "QuickPID.h"

class HoldPositionProcessor
{
private:
    Chute &m_chute;
    Joystick &m_joystick;
    LimitSwitch &m_cwLimit;
    LimitSwitch &m_ccwLimit;
    Motor &m_motor;

    
    float m_Kp = 0.02;
    float m_Ki = 0.002;
    float m_Kd = 0;
    float m_input;
    double getShortestAngleDifference(double target, double current);

public:
    HoldPositionProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor);
    void update(bool verbose);
    bool checkIfCurrentMode();
    void disableHoldPosition();
    void transitionToHold();

    float input() const;

};