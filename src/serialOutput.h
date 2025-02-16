#pragma once
#include "mode.h"

class SerialOutput
{
private:

    unsigned long lastPrint = 0;

public:
    void printMainLoopData(bool isManualmode, Mode currentMode, float output, float joystickPosition, float targetPosition, float currentPosition, float error, bool cwLimitHit, bool ccwLimitHit);
};