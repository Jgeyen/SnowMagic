#pragma once
#include "imu.h"

class Chute
{
private:
    float m_targetPosition;
    float m_totalRange;
    double alpha = 0.8; // Smoothing factor (0 < alpha <= 1)
    double filteredInput = 0.0;
    IMU &m_chuteIMU; // Reference to the IMU instance
    float m_filteredPosition;

public:
    Chute(IMU &imu);
    void Initialize();
    void update();
    bool isPositionValid();
    float currentPosition();
    float targetPosition() { return m_targetPosition; }
    float totalRange() { return m_totalRange; }
    void captureTargetPosition();
};