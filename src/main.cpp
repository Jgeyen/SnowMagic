#include "Arduino.h"
#include "imu.h"
#include "motor.h"
#include "chute.h"
#include "limitSwitch.h"
#include "joystick.h"
#include "processors/processor.h"
#include "constants.h"

// Globals
LimitSwitch ccwLimit(ccwPin);
LimitSwitch cwLimit(cwPin);
Joystick joystick;
Motor motor;
IMU chuteIMU;
Chute chute(chuteIMU);
Processor processor(chute, joystick, cwLimit, ccwLimit, motor);

PIDParameters pidParams = {0.02, 0.002, 0};

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setup starting");

  chuteIMU.initialize();

  ccwLimit.initialize();
  cwLimit.initialize();
  joystick.initialize();

  chute.initialize();
  motor.initialize();
  processor.initialize();

  pinMode(A0, INPUT);
}

void loop()
{
  chute.update();

  float motorSpeed = processor.update(false, pidParams);

  motor.setMotorSpeed(motorSpeed);

  // Add battery voltage sensing logic here
  // Serial.print(analogRead(A0));
  // Serial.print("; ");
  // Serial.println(analogRead(A1));
  // Add led display logic here
}
