#include "joystick.h"
#include "Arduino.h"
#include "constants.h"

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Joystick::Joystick(){

}

void Joystick::Initialize(){
    pinMode(m_analogPin, INPUT);
    pinMode(m_buttonPin, INPUT_PULLUP);
}

float Joystick::currentValue(){
    return analogRead(m_analogPin);
}
float Joystick::value(){

    // Apply dead zone
    if (!this->isActive()) {
        return 0.0f; // Within dead zone
    }

    float normalizedValue;
    if (this->isPushedLeft()) {
        // Left movement
        normalizedValue = fmap(currentValue(), minValue, centerValue - deadZone, -1, 0 );
    } else {
        // Right movement
        normalizedValue = fmap(currentValue(), centerValue - deadZone, maxValue, 0, 1 );
    }

    // Clamp to -1.0 to 1.0
    normalizedValue = constrain(normalizedValue, -1.0f, 1.0f);

    return normalizedValue;
}

bool Joystick::isActive(){
    int deviation = currentValue() - centerValue;
    // Apply dead zone
    return abs(deviation) > deadZone;
}

bool Joystick::isButtonPressed(){
    return !digitalRead(m_buttonPin);
}

bool Joystick::isPushedLeft(){
    
    return currentValue() < centerValue - deadZone;
}

bool Joystick::isPushedRight(){
    return currentValue() > centerValue + deadZone;
}