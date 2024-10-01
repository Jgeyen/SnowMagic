#include "imu.h"
#include <Wire.h>
#include "Arduino-ICM20948.h"


ArduinoICM20948Settings icmSettings = {
    .i2c_speed = 115200,               // i2c clock speed
    .is_SPI = false,                   // Enable SPI, if disable use i2c
    .cs_pin = 10,                      // SPI chip select pin
    .spi_speed = 7000000,              // SPI clock speed in Hz, max speed is 7MHz
    .mode = 1,                         // 0 = low power mode, 1 = high performance mode
    .enable_gyroscope = true,          // Enables gyroscope output
    .enable_accelerometer = true,      // Enables accelerometer output
    .enable_magnetometer = true,       // Enables magnetometer output // Enables quaternion output
    .enable_gravity = true,            // Enables gravity vector output
    .enable_linearAcceleration = true, // Enables linear acceleration output
    .enable_quaternion6 = true,        // Enables quaternion 6DOF output
    .enable_quaternion9 = true,        // Enables quaternion 9DOF output
    .enable_har = true,                // Enables activity recognition
    .enable_steps = true,              // Enables step counter
    .gyroscope_frequency = 1,          // Max frequency = 225, min frequency = 1
    .accelerometer_frequency = 1,      // Max frequency = 225, min frequency = 1
    .magnetometer_frequency = 1,       // Max frequency = 70, min frequency = 1
    .gravity_frequency = 1,            // Max frequency = 225, min frequency = 1
    .linearAcceleration_frequency = 1, // Max frequency = 225, min frequency = 1
    .quaternion6_frequency = 50,       // Max frequency = 225, min frequency = 50
    .quaternion9_frequency = 50,       // Max frequency = 225, min frequency = 50
    .har_frequency = 50,               // Max frequency = 225, min frequency = 50
    .steps_frequency = 50              // Max frequency = 225, min frequency = 50

};
static const uint8_t number_i2c_addr = 2;
uint8_t poss_addresses[number_i2c_addr] = {0X69, 0X68};
uint8_t ICM_address;
ArduinoICM20948 chuteIMU;
float ChuteYaw = 0;
bool ChuteIMUConnected = false;

void i2c_scan()
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
        ChuteIMUConnected = true;
      }
    }
  }
}

void InitializeChuteIMU()
{
  Serial.println("Initialize ICM");

  Wire.begin();
  Wire.setClock(1024);
  Serial.println("Starting ICM");
  delay(10);
  i2c_scan();
  if (ChuteIMUConnected)
  {
    Serial.println("ICM Found");
    chuteIMU.init(icmSettings);
  }
  else
  {
    Serial.println("ICM Not Found");
  }
}

void UpdateChuteIMU()
{
  float roll, pitch, yaw;
  char sensor_string_buff[128];
  chuteIMU.task();

  if (chuteIMU.euler6DataIsReady())
  {
    chuteIMU.readEuler6Data(&roll, &pitch, &yaw);
    ChuteYaw = yaw;
  }
}

