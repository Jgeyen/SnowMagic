#pragma once
#include "processors/processor.h"

class SerialInput
{
private:
public:
    PIDParameters processInput(PIDParameters params);
};