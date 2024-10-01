#include "limitSwitch.h"
#include "Arduino.h"

LimitSwitch::LimitSwitch(int ioPin){
    m_pin = ioPin;
}

void LimitSwitch::Initialize(){
    pinMode(m_pin, INPUT_PULLUP);
}

bool LimitSwitch::isHit(){
    return digitalRead(m_pin);
}