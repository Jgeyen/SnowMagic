// #include "Arduino.h";
// #include "imu.h";
// #include "motor.h";
// #include "chute.h";
// #include "limitSwitch.h";
// #include "joystick.h";
// #include "brain.h";
// #include "constants.h";

// // Globals
// LimitSwitch ccwLimit(ccwPin);
// LimitSwitch cwLimit(cwPin);
// Joystick joystick(joystickPin);
// Chute chute;

// void setup()
// {
//   Serial.begin(115200);
//   delay(10000);
//   while (!Serial)
//   Serial.println("Setup starting");

//   InitializeChuteIMU();
//   while (ChuteIMUConnected && ChuteYaw == 0)
//   {
//     UpdateChuteIMU();
//   }
//   ccwLimit.Initialize();
//   cwLimit.Initialize();
//   joystick.Initialize();

//   chute.Initialize();
//   initializeMotor(pwmPin, dirPin);
// }

// void loop()
// {

//   float motorSpeed = 0;

//   if (ChuteIMUConnected)
//   {
//     UpdateChuteIMU();
//     motorSpeed = Think(chute, joystick, cwLimit, ccwLimit);
//   }
//   else
//   {
//     motorSpeed = 0;
//   }
//   //setMotorSpeed(motorSpeed);

//   // Add battery voltage sensing logic here

//   // Add led display logic here
  
//   delay(100);
// }