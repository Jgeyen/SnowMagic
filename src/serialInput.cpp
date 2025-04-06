#include "Arduino.h"
#include "serialInput.h"


SerialInput::SerialInput() {
    m_receivedChars[0] = '\0'; // Ensure buffer is initially empty
}


bool SerialInput::recvWithEndMarker()
{
  char endMarker = '\n';
  char stopSerial = '`';
  char rc;

  while (Serial.available() > 0)
  {
    rc = Serial.read();

    if (rc == stopSerial)
    {
      m_verbose = !m_verbose;
    }
    else
    {
      if (rc != endMarker)
      {
        m_receivedChars[m_ndx] = rc;
        m_ndx++;
        if (m_ndx >= numChars)
        {
          m_ndx = numChars - 1;
        }
      }
      else
      {
        m_receivedChars[m_ndx] = '\0'; 
        m_ndx = 0;
        return true;
      }
    }
  }
  return false;
}

// Function to process the command and update PID parameters, returning a struct
PIDParameters SerialInput::processInput(PIDParameters params)
{
  // Call the member method
  if (this->recvWithEndMarker())
  {
    // Use the member buffer
    char *command = m_receivedChars;
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
      Serial.println("Invalid command");
    }

    return params; 
  }
  return params;
}
bool SerialInput::getVerbose() {
    return m_verbose;
}