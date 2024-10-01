#pragma once

class Chute
{
private:
    float m_targetPosition;
    float m_totalRange;

public:
    Chute();
    void Initialize();
    float currentPosition();
    float targetPosition() { return m_targetPosition; }
    float totalRange() {return m_totalRange;}
    void captureTargetPosition();
};
