// #include <Arduino.h>
// #include "constants.h"


// enum Direction {
//     CW,  // Clockwise
//     CCW  // Counter-Clockwise
// };

// void setDirection(Direction direction){
//   if(direction == CW){
//     digitalWrite(dirPin, LOW);
//   }else{
//     digitalWrite(dirPin, HIGH);
//   }
// }

// void setup() {
//   pinMode(pwmPin, OUTPUT);
//   pinMode(dirPin, OUTPUT);
//   pinMode(ccwPin, INPUT_PULLUP);
//   pinMode(cwPin, INPUT_PULLUP);
//   pinMode(joystickButtonPin, INPUT_PULLUP);
  
//   Serial.begin(115200); // Start Serial communication at 9600 baud rate
// }

// void loop() {
//   int joystickValue = analogRead(joystickPin);  // Read the joystick value (0-1023)
//   int voltage = analogRead(voltageReadingPin);

//   int pwmOutput = 0;
//   Direction currentDirection = CW;

//   if(joystickValue < deadZoneLower) {
//     // Joystick moved to the left side
//     currentDirection = CCW;
//     pwmOutput = map(joystickValue, 0, deadZoneLower, maxSpeed, 0);
//   } 
//   else if(joystickValue > deadZoneUpper) {
//     // Joystick moved to the right side
//     currentDirection = CW;
//     pwmOutput = map(joystickValue, deadZoneUpper, 1023, 0, maxSpeed);
//   } 
//   else {
//     // Joystick is in the dead zone, stop the motor
//     pwmOutput = 0;
//   }

//   bool ccw_limit = digitalRead(ccwPin);
//   bool cw_limit = digitalRead(cwPin);
//   bool modeButton = digitalRead(joystickButtonPin);

//   setDirection(currentDirection);

//   if((ccw_limit && currentDirection == CCW) || (cw_limit && currentDirection == CW)){
//     pwmOutput = 0;
//   } 
//   analogWrite(pwmPin, pwmOutput);


//   // Send the data over Serial
//    Serial.print("Joystick Position: "); Serial.print(joystickValue);
//   Serial.print(", Direction: "); Serial.print(currentDirection);
//   Serial.print(", PWM Output: "); Serial.println(pwmOutput);
//   Serial.print(", cw: " );Serial.println(cw_limit == HIGH);
//   Serial.print(", ccw: " );Serial.println(ccw_limit== HIGH);
//   Serial.print(", button: " );Serial.println(modeButton== HIGH);
//   Serial.print(", voltage: ");Serial.println(voltage);
//   delay(20);  // Short delay for better loop timing
// }

