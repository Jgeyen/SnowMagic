#include "Arduino.h"
#include "imu.h"
#include "motor.h"
#include "chute.h"
#include "limitSwitch.h"
#include "joystick.h"
#include "brain.h"
#include "constants.h"

// Globals
LimitSwitch ccwLimit(ccwPin);
LimitSwitch cwLimit(cwPin);
Joystick joystick;
Motor motor;
Brain brain;
IMU chuteIMU;
Chute chute(chuteIMU);

PIDParameters pidParams = {0.02, 0.002, 0};

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setup starting");

  chuteIMU.initialize();

  ccwLimit.Initialize();
  cwLimit.Initialize();
  joystick.Initialize();

  chute.Initialize();
  motor.Initialize();
  brain.Initialize();

  pinMode(A0, INPUT);
}

void loop()
{
  chute.update();

  float motorSpeed = brain.Think(chute, joystick, cwLimit, ccwLimit, false, pidParams);

  motor.SetMotorSpeed(motorSpeed);

  // Add battery voltage sensing logic here
  // Serial.print(analogRead(A0));
  // Serial.print("; ");
  // Serial.println(analogRead(A1));
  // Add led display logic here
}
