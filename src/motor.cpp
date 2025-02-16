#include "Arduino.h"
#include "motor.h"
#include "constants.h"

void Motor::initialize()
{
    pinMode(m_motorPin, OUTPUT);
    pinMode(m_directionPin, OUTPUT);
}

void Motor::setMotorSpeed(float output)
{
    if (output < 0)
    {
        m_direction = Direction::CCW;
    }
    else
    {
        m_direction = Direction::CW;
    }

    float motorSpeed = m_maxDutyCycle * abs(output);

    setDirection(m_direction);
    analogWrite(m_motorPin, constrain(motorSpeed, 0, m_maxDutyCycle));
}
void Motor::setDirection(Direction direction)
{
    if (direction == Direction::CW)
    {
        digitalWrite(m_directionPin, LOW);
    }
    else
    {
        digitalWrite(m_directionPin, HIGH);
    }
}

float Motor::speed() const {
    return m_speed;
}
