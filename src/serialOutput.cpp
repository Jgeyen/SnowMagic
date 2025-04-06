#include "serialOutput.h"
#include "shared.h"

void SerialOutput::printMainLoopData(
    Mode currentMode,
    Motor &motor,
    Joystick &joystick,
    Chute &chute,
    HoldPositionProcessor &holdProcessor,
    LimitSwitch &cwLimit,
    LimitSwitch &ccwLimit)
{
    if (!Shared::isTimeElapsed(lastPrint, 500))
    {
        return;
    }

    Serial.print("t:");
    Serial.print(static_cast<float>(millis()) / 1000);
    Serial.print(" | cm:");
    switch (currentMode)
    {
    case Mode::ManualControl:
        Serial.print("Manual");
        break;
    case Mode::Startup:
        Serial.print("Startup");
        break;
    case Mode::TransitionToHold:
        Serial.print("Transition");
        break;
    case Mode::HoldPosition:
        Serial.print("Hold");
        break;
    case Mode::AtCWLimit:
        Serial.print("AtCWLimit");
        break;
    case Mode::AtCCWLimit:
        Serial.print("AtCCWLimit");
        break;
    case Mode::TraverseCCWToTP:
        Serial.print("TraverseCCW");
        break;
    case Mode::TraverseCWToTP:
        Serial.print("TraverseCW");
        break;
    case Mode::Error:
        Serial.print("Error");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
    Serial.print(" | cwl:");
    Serial.print(cwLimit.isHit());
    Serial.print(" | ccwl:");
    Serial.print(ccwLimit.isHit());
    Serial.print(" | cp:");
    Serial.print(chute.currentPosition());

    // --- Mode-Specific Info ---
    switch (currentMode)
    {
    case Mode::ManualControl:
        Serial.print(" | jp:");
        if (joystick.value() >= 0) Serial.print("+");
        Serial.print(joystick.value());
        Serial.print(" | mot_spd:");
        if (motor.speed() >= 0) Serial.print("+");
        Serial.print(motor.speed());
        break;

    case Mode::HoldPosition:
        Serial.print(" | tp:");
        
        if (chute.targetPosition() >= 0) Serial.print("+");
        Serial.print(chute.targetPosition());
        Serial.print(" | er:");
        if (holdProcessor.input() >= 0) Serial.print("+");
        Serial.print(holdProcessor.input());
        Serial.print(" | pid_out:");
        if (holdProcessor.getOutput() >= 0) Serial.print("+");
        Serial.print(holdProcessor.getOutput());
        Serial.print(" | Kp:");
        if (holdProcessor.getKp() >= 0) Serial.print("+");
        Serial.print(holdProcessor.getKp());
        Serial.print(" | Ki:");
        if (holdProcessor.getKi() >= 0) Serial.print("+");
        Serial.print(holdProcessor.getKi());
        Serial.print(" | Kd:");
        if (holdProcessor.getKd() >= 0) Serial.print("+");
        Serial.print(holdProcessor.getKd());
        break;

    case Mode::TransitionToHold:
         Serial.print(" | tp:");
         if (chute.targetPosition() >= 0) Serial.print("+");
         Serial.print(chute.targetPosition());
        Serial.print(" | pid_out:");
        if (holdProcessor.getOutput() >= 0) Serial.print("+");
        Serial.print(holdProcessor.getOutput());
        break;

    case Mode::Startup:
    case Mode::AtCWLimit:
    case Mode::AtCCWLimit:
    case Mode::TraverseCCWToTP:
    case Mode::TraverseCWToTP:
    case Mode::Error:
    default:
        Serial.print(" | mot_spd:");
        if (motor.speed() >= 0) Serial.print("+");
        Serial.print(motor.speed());
        break;
    }

    Serial.println();
}