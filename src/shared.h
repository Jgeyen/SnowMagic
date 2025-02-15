#pragma once

#include "Arduino.h"

class Shared
{
private:
public:
    static bool isTimeElapsed(unsigned long &lastUpdateTime, unsigned long update_interval_ms);
};