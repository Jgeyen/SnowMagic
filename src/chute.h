#pragma once

class Chute
{
private:
    float m_targetPosition;
    float m_totalRange;
    double alpha = 0.8; // Smoothing factor (0 < alpha <= 1)
    double filteredInput = 0.0;

public:
    Chute();
    void Initialize();
    float currentPosition();
    float targetPosition() { return m_targetPosition; }
    float totalRange() {return m_totalRange;}
    void captureTargetPosition();
};
