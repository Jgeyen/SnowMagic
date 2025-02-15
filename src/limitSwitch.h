#pragma once

class LimitSwitch
{
private:
    int m_pin;

public:
    LimitSwitch(int pin);
    void initialize();
    bool isHit();
};
