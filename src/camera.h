#include <esp_camera.h>
// details at https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/esp32/include/esp32-camera/driver/include/esp_camera.h

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     4
#define SIOD_GPIO_NUM     18
#define SIOC_GPIO_NUM     23
#define Y7_GPIO_NUM       36
#define Y6_GPIO_NUM       37
#define Y5_GPIO_NUM       38
#define Y4_GPIO_NUM       39
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       14
#define Y1_GPIO_NUM       13
#define Y0_GPIO_NUM       34
#define VSYNC_GPIO_NUM    5
#define HREF_GPIO_NUM     27
#define PCLK_GPIO_NUM     25

/**
Name	T-Camera V162
DVP Y9	36
DVP Y8	37
DVP Y7	38
DVP Y6	39
DVP Y5	35
DVP Y4	14
DVP Y3	13
DVP Y2	34
DVP VSNC	5
DVP HREF	27
DVP PCLK	25
DVP PWD	N/A
DVP XCLK	4
DVP SIOD	18
DVP SIOC	23
DVP RESET	N/A
SDA	21
SCL	22
Button	15
PIR	19
OLED Model	SSD1306
TFT Width	128
TFT Height	64
IIS SCK	26
IIS WS	32
IIS DOUT	33
*/

camera_config_t config {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y7_GPIO_NUM,
    .pin_d6 = Y6_GPIO_NUM,
    .pin_d5 = Y5_GPIO_NUM,
    .pin_d4 = Y4_GPIO_NUM,
    .pin_d3 = Y3_GPIO_NUM,
    .pin_d2 = Y2_GPIO_NUM,
    .pin_d1 = Y1_GPIO_NUM,
    .pin_d0 = Y0_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG, // https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/esp32/include/esp32-camera/driver/include/sensor.h#L57
    .frame_size = FRAMESIZE_XGA,    // https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/esp32/include/esp32-camera/driver/include/sensor.h#L68
    .jpeg_quality = 5,              // 0-63, lower = better
    .fb_count = 2                   // Number of frame buffers to be allocated. If more than one, then each frame will be acquired (double speed)
};
