#pragma once
#ifdef ARDUINO
  #include <Arduino-ICM20948.h>
  #include <Wire.h>
#else
#include "../test/mocks/Arduino-ICM20948.h"
#endif

#include "shared.h"

class IMU
{
private:
    void i2c_scan();
    void pollIMU();

    const unsigned long UPDATE_INTERVAL_MS = 10; // 10 ms = 1/100 second (100 Hz)
    unsigned long m_lastUpdateTime = 0;
    static const uint8_t number_i2c_addr = 2;
    uint8_t poss_addresses[number_i2c_addr] = {0X69, 0X68};
    uint8_t m_ICM_address;
    ArduinoICM20948 m_imu;

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

public:
    float yaw;
    bool isConnected;
    void initialize();
    void updateValues();
};