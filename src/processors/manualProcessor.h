#pragma once
#include "limitSwitch.h"
#include "chute.h"
#include "joystick.h"
#include "motor.h"

class ManualProcessor
{
private:
    Chute &m_chute;
    Joystick &m_joystick;
    LimitSwitch &m_cwLimit;
    LimitSwitch &m_ccwLimit;
    Motor &m_motor;
    bool m_buttonPressLatch = false;
    

    bool userSelectedManualMode();

public:
    ManualProcessor(Chute &chute, Joystick &joystick, LimitSwitch &cwLimit, LimitSwitch &ccwLimit, Motor &motor);
    void update(bool verbose);
    bool checkIfCurrentMode();
    bool isManual = true;
    
};