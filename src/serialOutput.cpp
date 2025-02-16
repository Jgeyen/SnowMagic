#include "serialOutput.h"
#include "shared.h"

void SerialOutput::printMainLoopData(bool isManualmode, Mode currentMode, float output, float joystickPosition, float targetPosition, float currentPosition, float error, bool cwLimitHit, bool ccwLimitHit)
{
  if (!Shared::isTimeElapsed(lastPrint, 500))
  {
    return;
  }
  Serial.print("t:");
  Serial.print(static_cast<float>(millis()) / 1000);

  // Serial.print ("mem:");
  // Serial.print(freeMemory());

  Serial.print("man:");
  Serial.print(isManualmode);

  Serial.print("; out:");
  Serial.print(output);

  Serial.print("; jp:");
  Serial.print(joystickPosition);

  Serial.print("; tp:");
  Serial.print(targetPosition);

  Serial.print(";cp:");
  Serial.print(currentPosition);

  Serial.print(";er:");
  Serial.print(error);

  Serial.print(";cwl:");
  Serial.print(cwLimitHit);

  Serial.print(";ccwl:");
  Serial.print(ccwLimitHit);

  Serial.print(";cm:");
  switch (currentMode)
  {
  case Mode::ManualControl:
    Serial.println("Manual");
    break;
  case Mode::Startup:
    Serial.println("Startup");
    break;
  case Mode::TransitionToHold:
    Serial.println("Transition");
    break;
  case Mode::HoldPosition:
    Serial.println("Hold");
    break;
  case Mode::AtCWLimit:
    Serial.println("AtCWLimit");
    break;
  case Mode::AtCCWLimit:
    Serial.println("AtCCWLimit");
    break;
  case Mode::TraverseCCWToTP:
    Serial.println("TraverseCCW");
    break;
  case Mode::TraverseCWToTP:
    Serial.println("TraverseCW");
    break;
  case Mode::Error:
    Serial.println("Error");
    break;
  default:
    Serial.println("Unknown");
    break;
  }
}