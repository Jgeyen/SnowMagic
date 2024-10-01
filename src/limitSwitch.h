#pragma once

class LimitSwitch
{
private:
    int m_pin;

public:
    LimitSwitch(int pin);
    void Initialize();
    bool isHit();
};
