#pragma once

enum class Direction : int
{
    CW, // Clockwise
    CCW // Counter-Clockwise
};

class Motor
{
private:
    Direction m_direction;
    const int m_motorPin = 5;
    const int m_directionPin = 4;
    int m_maxDutyCycle = 255; // Max value is 255
    float m_speed;
    void setDirection(Direction);

public:
    void initialize();
    void setMotorSpeed(float);

    float speed() const;
};
