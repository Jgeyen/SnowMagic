#include "Arduino.h";
#include "imu.h";
#include "motor.h";
#include "chute.h";
#include "limitSwitch.h";
#include "joystick.h";
#include "brain.h";
#include "constants.h";

// Globals
LimitSwitch ccwLimit(ccwPin);
LimitSwitch cwLimit(cwPin);
Joystick joystick;
Motor motor;
Brain brain;
Chute chute;

const byte numChars = 32;
char receivedChars[numChars];
boolean verbose = true;
boolean newData = false;
PIDParameters pidParams = {0.02,0,0};

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char stopSerial = '`';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if(rc == stopSerial){
          verbose = !verbose;
        }else{
          if (rc != endMarker) {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) {
                  ndx = numChars - 1;
              }
          }
          else {
              receivedChars[ndx] = '\0'; // terminate the string
              ndx = 0;
              newData = true;
          }
        }
    }
}


// Function to process the command and update PID parameters, returning a struct
PIDParameters processCommand(char* command) {
  PIDParameters params = pidParams;  // Create a new instance of the struct

  // Check for the first character and parse the corresponding value
  if (command[0] == 'P' || command[0] == 'p') {
    params.proportional = atof(&command[1]);  // Convert the value after 'P' to float
    Serial.print("********** Proportional set to: ");
    Serial.println(params.proportional);
  } else if (command[0] == 'I' || command[0] == 'i') {
    params.integral = atof(&command[1]);  // Convert the value after 'I' to float
    Serial.print("********** Integral set to: ");
    Serial.println(params.integral);
  } else if (command[0] == 'D' || command[0] == 'd') {
    params.derivative = atof(&command[1]);  // Convert the value after 'D' to float
    Serial.print("********** Derivative set to: ");
    Serial.println(params.derivative);
  } else {
    Serial.println("Invalid command");  // Handle unrecognized commands
  }

  return params;  // Return the updated struct
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  while (!Serial)
  Serial.println("Setup starting");

  InitializeChuteIMU();
  while (ChuteIMUConnected && ChuteYaw == 0)
  {
    UpdateChuteIMU();
  }
  ccwLimit.Initialize();
  cwLimit.Initialize();
  joystick.Initialize();

  chute.Initialize();
  motor.Initialize();
  brain.Initialize();

  
}

void loop()
{

  // MotorParams motorParams;
  float brainOutput = 0;

  if (ChuteIMUConnected)
  {
    recvWithEndMarker();
    if (newData == true) {
      pidParams = processCommand(receivedChars);
      newData = false;
      verbose = true;
    }
    UpdateChuteIMU();
    brainOutput = brain.Think(chute, joystick, cwLimit, ccwLimit, verbose, pidParams);
  }
  else
  {
    brainOutput = 0;
  }
  motor.SetMotorSpeed(brainOutput);

  // Add battery voltage sensing logic here

  // Add led display logic here
  
  delay(1);
}
