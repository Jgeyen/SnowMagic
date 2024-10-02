#include "chute.h"
#include "imu.h"
#include "Arduino.h"

Chute::Chute(){
  m_targetPosition = 0;
  m_totalRange = 220;
}

float Chute::currentPosition(){
  filteredInput = alpha * ChuteYaw + (1 - alpha) * filteredInput;
  return filteredInput;
}

void Chute::captureTargetPosition(){
  m_targetPosition = ChuteYaw;
}

void Chute::Initialize(){
  Serial.print("Initialize Chute");
  Serial.println(ChuteYaw);
}