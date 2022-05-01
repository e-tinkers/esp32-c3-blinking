/* This is the demonstration program used for e-tinkers-esp32-c3 board */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "led_strip.h"

#define RGB_PIN 8
static led_strip_t *rgb;
static uint8_t rgb_state = 0;

// data sequence in WS2812 is GRB instead of RGB
uint8_t colors[][3] = {
  {255, 255, 255}, // white
  {255, 0,   0},   // green
  {0,   255, 0},   // red
  {0,   0,   255}, // blue
  {255, 255, 0},   // yellow
  {0,   255, 255}, // magenta
  {255, 0,   255}  // cyan
};

void app_main(void) {
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("\n%s with %d CPU core(s), Revision: %d, WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            chip_info.revision,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    rgb = led_strip_init(0, RGB_PIN, 1);    // configure RGB
    rgb->clear(rgb, 50);    //clear all LEDs

    uint8_t n = 0;
    while(1) {
        if (rgb_state) {
          rgb->set_pixel(rgb, 0, colors[n][0], colors[n][1], colors[n][2]);
          rgb->refresh(rgb, 100);    // reflash RGB LED to send data
          n = (n + 1) % 7;
        }
        else {
          rgb->clear(rgb, 50);
        }
        rgb_state = !rgb_state;
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
