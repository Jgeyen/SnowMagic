#include "imu.h"

void IMU::i2c_scan()
{
  uint8_t error;
  for (uint8_t add_int = 0; add_int < number_i2c_addr; add_int++)
  {
    Serial.print("Scanning 0x%02X for slave...");
    Serial.println(poss_addresses[add_int]);
    Wire.beginTransmission(poss_addresses[add_int]);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.println("found.");
      if (poss_addresses[add_int] == 0x69 || poss_addresses[add_int] == 0x68)
      {
        Serial.println("\t- address is ICM.");
        ICM_address = poss_addresses[add_int];
        isConnected = true;
      }
    }
  }
}

void IMU::initialize()
{
  Serial.println("Initialize ICM");

  Wire.begin();
  Wire.setClock(1024);
  Serial.println("Starting ICM");
  delay(10);
  i2c_scan();
  if (isConnected)
  {
    Serial.println("ICM Found");
    chuteIMU.init(icmSettings);
  }
  else
  {
    Serial.println("ICM Not Found");
  }
}

void IMU::updateValues()
{
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL_MS)
  {
    lastUpdateTime = currentTime;
    // Pull the update from the ICM20948
    pollIMU();
  }
}

void IMU::pollIMU()
{
  float roll, pitch, yaw;
  char sensor_string_buff[128];

  if (chuteIMU.euler6DataIsReady())
  {
    chuteIMU.readEuler6Data(&roll, &pitch, &yaw);
    yaw = yaw;
  }
}
