#include "chute.h"
#include "imu.h"
#include "Arduino.h"

Chute::Chute(IMU &imu)
    : m_chuteIMU(imu), m_filteredPosition(0.0f), m_targetPosition(0.0f), m_totalRange(220.0f) {}

void Chute::update()
{
  m_chuteIMU.updateValues();
}

bool Chute::isPositionValid()
{
  return m_chuteIMU.isConnected;
}

float Chute::currentPosition()
{
  filteredInput = alpha * m_chuteIMU.yaw + (1 - alpha) * filteredInput;
  return filteredInput;
}

void Chute::captureTargetPosition()
{
  m_targetPosition = m_chuteIMU.yaw;
}

void Chute::initialize()
{
  Serial.print("Initialize Chute");
  Serial.println(m_chuteIMU.yaw);
}

float Chute::targetPosition() { return m_targetPosition; }
float Chute::totalRange() { return m_totalRange; }