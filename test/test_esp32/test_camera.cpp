#include <Arduino.h>

#include <esp_camera.h>
#include <unity.h>

#include <camera.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_initialize_camera(void) {
  // Init with config
  esp_err_t err = esp_camera_init(&config);
  TEST_ASSERT_EQUAL(ESP_OK, err);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_initialize_camera);
  UNITY_END();
}

void loop() {
  delay(5000);
}
