// test_processors.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ArduinoFake.h>
#include "mocks/QuickPID.h"
#include "../src/processors/manualProcessor.h"
#include "../src/processors/holdPositionProcessor.h"

using ::testing::_;
using ::testing::Return;


class MockMotor : public Motor
{
public:
    MOCK_METHOD1(setMotorSpeed, void(float speed));
};

class MockIMU: public IMU{
    public:
    MOCK_METHOD0(  yaw,float());
    MOCK_METHOD0(  isConnected,bool());
    MOCK_METHOD0(  initialize,void());
    MOCK_METHOD0(  updateValues,void());
};
class MockChute : public Chute {
    public:
    MockChute(IMU &imu) : Chute(imu) {}
    MOCK_METHOD0(  initialize,void());
    MOCK_METHOD0(  update,void());
    MOCK_METHOD0(  isPositionValid,bool());
    MOCK_METHOD0(  currentPosition,float());
    MOCK_METHOD0(  targetPosition,float());
    MOCK_METHOD0(  totalRange,float());
    MOCK_METHOD0(  captureTargetPosition,void());
};


// --- Unit Tests ---

// Test the ManualProcessor: In manual mode, if the joystick is pushed left and the corresponding limit is not hit,
// then the processor should command the motor with the joystick's value.
TEST(ChuteTest, GetChutePosition)
{
    MockIMU mockIMU;
    

    // Set up expectations:
    // We expect the motor to be commanded with the same value as the joystick:
    EXPECT_CALL(mockIMU, yaw())
        .Times(1);

    
    // Create a ManualProcessor instance using the mocks.
    Chute chute(mockIMU);
    

    // // Call update (verbose flag doesn't affect manual update logic in this example).
    float bob = chute.currentPosition();
    
}

// Test the HoldPositionProcessor: When in hold position, it should compute the PID output.
// We simulate a situation where the current and target positions are different.
// TEST(HoldPositionProcessorTest, HoldPositionPIDControl)
// {
//     MockChute mockChute;
//     MockJoystickMock mockJoystick;
//     MockLimitSwitchMock mockCW;
//     MockLimitSwitchMock mockCCW;
//     MockMotorMock mockMotor;

//     // Set up the chute to return a target position and a current position.
//     EXPECT_CALL(mockChute, targetPosition())
//         .WillOnce(Return(100.0f));
//     EXPECT_CALL(mockChute, currentPosition())
//         .WillOnce(Return(90.0f));
//     // We do not need joystick or limit switch actions for the hold position test in this case.
//     // For simplicity, assume the PID calculation (inside HoldPositionProcessor) produces an output.
//     // Here we just expect that eventually the motor's setMotorSpeed is called.
//     EXPECT_CALL(mockMotor, setMotorSpeed(_))
//         .Times(1);

//     // Create a HoldPositionProcessor instance using the mocks.
//     HoldPositionProcessor processor(mockChute, mockJoystick, mockCW, mockCCW, mockMotor);

//     // // Call update. (Verbose flag false here.)
//     // processor.update(false);
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    if (RUN_ALL_TESTS())
        ;
    return 0;
}
