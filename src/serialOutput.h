#pragma once
#include "mode.h"
#include "motor.h"
#include "joystick.h"
#include "chute.h"
#include "processors/holdPositionProcessor.h"
#include "limitSwitch.h"

class SerialOutput
{
private:

    unsigned long lastPrint = 0;

public:
    void printMainLoopData(
        Mode currentMode,
        Motor &motor,
        Joystick &joystick,
        Chute &chute,
        HoldPositionProcessor &holdProcessor,
        LimitSwitch &cwLimit,
        LimitSwitch &ccwLimit
    );
};