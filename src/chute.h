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
    virtual void initialize();
    virtual void update();
    virtual bool isPositionValid();
    virtual float currentPosition();
    virtual float targetPosition();
    virtual float totalRange();
    virtual void captureTargetPosition();
};