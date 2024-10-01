
#include <unity.h>

void test_function_multiply(void) {
    TEST_ASSERT_EQUAL(10, 5 * 2);
}

void setup() {
    delay(2000);  // 2 second delay for the serial connection
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_function_multiply);
    UNITY_END(); // stop unit testing
}

void loop() {
}
