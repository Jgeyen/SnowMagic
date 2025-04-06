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

    // PID variables moved from global scope
    float m_setpoint;
    float m_input;    // Represents the error (difference) calculated
    float m_output;   // PID controller output
    QuickPID m_pid;   // PID controller instance

    // Helper function
    double getShortestAngleDifference(double target, double current);

public:
    HoldPositionProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor);
    void update(bool verbose);
    // bool checkIfCurrentMode();
    void disableHoldPosition();
    void transitionToHold();

    float input() const; // Returns the calculated error/input to PID
    float getOutput() const; // Returns the calculated PID output
    // Add getters for PID parameters if needed for printing
    // Removed const because QuickPID::GetKx() methods are likely not const
    float getKp();
    float getKi();
    float getKd();
};