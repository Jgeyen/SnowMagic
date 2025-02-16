// test_processors.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ArduinoFake.h>
#include "../src/processors/manualProcessor.h"
#include "../src/processors/holdPositionProcessor.h"

using ::testing::Return;
using ::testing::_;

// --- Mock Classes ---
// These mocks replicate the interfaces of your dependencies.
// In a production code base you might have abstract interfaces to derive these mocks from.

class MockChute {
public:
    // Mark methods as virtual if you plan to override them.
    virtual ~MockChute() {}
    virtual float currentPosition() const { return 0.0f; }
    virtual float targetPosition() const { return 0.0f; }
    virtual float totalRange() const { return 0.0f; }
    virtual void captureTargetPosition() {}
    virtual bool isPositionValid() const { return true; }
};

class MockJoystick {
public:
    virtual ~MockJoystick() {}
    virtual float value() const { return 0.0f; }
    virtual bool isActive() const { return false; }
    virtual bool isButtonPressed() const { return false; }
    virtual bool isPushedLeft() const { return false; }
    virtual bool isPushedRight() const { return false; }
};

class MockLimitSwitch {
public:
    virtual ~MockLimitSwitch() {}
    virtual bool isHit() const { return false; }
};

class MockMotor {
public:
    virtual ~MockMotor() {}
    virtual void setMotorSpeed(float) {}
};

// Now, use Google Mock to create mocks that inherit from these classes:
class MockChuteMock : public MockChute {
public:
    MOCK_METHOD(float, currentPosition, (), (const, override));
    MOCK_METHOD(float, targetPosition, (), (const, override));
    MOCK_METHOD(float, totalRange, (), (const, override));
    MOCK_METHOD(void, captureTargetPosition, (), (override));
    MOCK_METHOD(bool, isPositionValid, (), (const, override));
};

class MockJoystickMock : public MockJoystick {
public:
    MOCK_METHOD(float, value, (), (const, override));
    MOCK_METHOD(bool, isActive, (), (const, override));
    MOCK_METHOD(bool, isButtonPressed, (), (const, override));
    MOCK_METHOD(bool, isPushedLeft, (), (const, override));
    MOCK_METHOD(bool, isPushedRight, (), (const, override));
};

class MockLimitSwitchMock : public MockLimitSwitch {
public:
    MOCK_METHOD(bool, isHit, (), (const, override));
};

class MockMotorMock : public MockMotor {
public:
    MOCK_METHOD(void, setMotorSpeed, (float), (override));
};

// --- Unit Tests ---

// Test the ManualProcessor: In manual mode, if the joystick is pushed left and the corresponding limit is not hit,
// then the processor should command the motor with the joystick's value.
TEST(ManualProcessorTest, ManualModeLeftMovement) {
    MockChuteMock mockChute;
    MockJoystickMock mockJoystick;
    MockLimitSwitchMock mockCW;
    MockLimitSwitchMock mockCCW;
    MockMotorMock mockMotor;

    // Set up expectations:
    EXPECT_CALL(mockJoystick, isPushedLeft())
        .WillOnce(Return(true));
    EXPECT_CALL(mockJoystick, value())
        .WillOnce(Return(-0.7f)); // Negative value indicates left movement
    EXPECT_CALL(mockCCW, isHit())
        .WillOnce(Return(false));
    // We expect the motor to be commanded with the same value as the joystick:
    EXPECT_CALL(mockMotor, setMotorSpeed(-0.7f))
        .Times(1);

    // Create a ManualProcessor instance using the mocks.
    ManualProcessor processor(mockChute, mockJoystick, mockCW, mockCCW, mockMotor);

    // Call update (verbose flag doesn't affect manual update logic in this example).
    processor.update(false);
}

// Test the HoldPositionProcessor: When in hold position, it should compute the PID output.
// We simulate a situation where the current and target positions are different.
TEST(HoldPositionProcessorTest, HoldPositionPIDControl) {
    MockChuteMock mockChute;
    MockJoystickMock mockJoystick;
    MockLimitSwitchMock mockCW;
    MockLimitSwitchMock mockCCW;
    MockMotorMock mockMotor;

    // Set up the chute to return a target position and a current position.
    EXPECT_CALL(mockChute, targetPosition())
        .WillOnce(Return(100.0f));
    EXPECT_CALL(mockChute, currentPosition())
        .WillOnce(Return(90.0f));
    // We do not need joystick or limit switch actions for the hold position test in this case.
    // For simplicity, assume the PID calculation (inside HoldPositionProcessor) produces an output.
    // Here we just expect that eventually the motor's setMotorSpeed is called.
    EXPECT_CALL(mockMotor, setMotorSpeed(_))
        .Times(1);

    // Create a HoldPositionProcessor instance using the mocks.
    HoldPositionProcessor processor(mockChute, mockJoystick, mockCW, mockCCW, mockMotor);

    // Call update. (Verbose flag false here.)
    processor.update(false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
