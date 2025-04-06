#include "limitSwitch.h"
#include "Arduino.h"

LimitSwitch::LimitSwitch(int ioPin)
{
    m_pin = ioPin;
}

void LimitSwitch::initialize()
{
    pinMode(m_pin, INPUT_PULLUP);
}

bool LimitSwitch::isHit()
{
    // For Normally Closed (NC) switches with INPUT_PULLUP:
    // - Not pressed (closed circuit) -> LOW
    // - Pressed (open circuit) -> HIGH
    // - Broken wire (open circuit) -> HIGH
    // Return true if pressed OR broken (HIGH)
    return digitalRead(m_pin);
}