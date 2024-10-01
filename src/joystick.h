#include <Arduino.h>
#pragma once


class Joystick
{
    
private:
    const int m_analogPin = A1;  // Joystick connected to analog pin A0
    const int m_buttonPin = 6;
    float currentValue();
    // const int deadZoneLower = 460; // Lower bound for joystick dead zone
    // const int deadZoneUpper = 540; // Upper bound for joystick dead zone
    // const int deadZoneValue = 500; // Upper bound for joystick dead zone
    const int minValue = 0;
    const int centerValue = 512; // Center value for analogRead (0-1023)
    const int maxValue = 1024;
    const int deadZone = 50;     // Dead zone threshold

public:
    Joystick();
    void Initialize();
    float value();
    bool isActive();
    bool isButtonPressed();
    bool isPushedLeft();
    bool isPushedRight();
};
