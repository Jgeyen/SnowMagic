#include "Arduino.h"
#include "serialInput.h"

const byte numChars = 32;
char receivedChars[numChars];
boolean verbose = true;
boolean newData = false;

bool recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char stopSerial = '`';
  char rc;

  while (Serial.available() > 0)
  {
    rc = Serial.read();

    if (rc == stopSerial)
    {
      verbose = !verbose;
    }
    else
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        ndx = 0;
        return true;
      }
    }
  }
  return false;
}

// Function to process the command and update PID parameters, returning a struct
PIDParameters SerialInput::processInput(PIDParameters params)
{
  if (recvWithEndMarker())
  {
    char *command = receivedChars;
    // Check for the first character and parse the corresponding value
    if (command[0] == 'P' || command[0] == 'p')
    {
      params.proportional = atof(&command[1]); // Convert the value after 'P' to float
      Serial.print("********** Proportional set to: ");
      Serial.println(params.proportional);
    }
    else if (command[0] == 'I' || command[0] == 'i')
    {
      params.integral = atof(&command[1]); // Convert the value after 'I' to float
      Serial.print("********** Integral set to: ");
      Serial.println(params.integral);
    }
    else if (command[0] == 'D' || command[0] == 'd')
    {
      params.derivative = atof(&command[1]); // Convert the value after 'D' to float
      Serial.print("********** Derivative set to: ");
      Serial.println(params.derivative);
    }
    else
    {
      Serial.println("Invalid command"); // Handle unrecognized commands
    }

    return params; // Return the updated struct
  }
  return params;
}