typedef struct {
    int i2c_speed;
    bool is_SPI;
    int cs_pin;
    int spi_speed;
    int mode;
    bool enable_gyroscope;
    bool enable_accelerometer;
    bool enable_magnetometer;
    bool enable_gravity;
    bool enable_linearAcceleration;
    bool enable_quaternion6;
    bool enable_quaternion9;
    bool enable_har;
    bool enable_steps;
    int gyroscope_frequency;
    int accelerometer_frequency;
    int magnetometer_frequency;
    int gravity_frequency;
    int linearAcceleration_frequency;
    int quaternion6_frequency;
    int quaternion9_frequency;
    int har_frequency;
    int steps_frequency;
  
  } ArduinoICM20948Settings;


class ArduinoICM20948 {
    public:
        bool init(const ArduinoICM20948Settings &settings) { return true; }
        bool euler6DataIsReady() const { return true; }
        void task() { }
        void readEuler6Data(float *roll, float *pitch, float *yaw) {
            // Provide dummy data for testing
            *roll = 0.0f;
            *pitch = 0.0f;
            *yaw = 45.0f;
        }
    };