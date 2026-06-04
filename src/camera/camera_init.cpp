#include "camera/camera_init.h"
#include "camera_pins.h"
#include "ledc_channels.h"
#include "esp_camera.h"
#include <esp32-hal-ledc.h>
#include <Arduino.h>

static camera_config_t g_config;

static void buildConfig() {
  g_config.ledc_channel = LEDC_CHANNEL_0;
  g_config.ledc_timer   = LEDC_TIMER_0;
  g_config.pin_d0       = Y2_GPIO_NUM;
  g_config.pin_d1       = Y3_GPIO_NUM;
  g_config.pin_d2       = Y4_GPIO_NUM;
  g_config.pin_d3       = Y5_GPIO_NUM;
  g_config.pin_d4       = Y6_GPIO_NUM;
  g_config.pin_d5       = Y7_GPIO_NUM;
  g_config.pin_d6       = Y8_GPIO_NUM;
  g_config.pin_d7       = Y9_GPIO_NUM;
  g_config.pin_xclk     = XCLK_GPIO_NUM;
  g_config.pin_pclk     = PCLK_GPIO_NUM;
  g_config.pin_vsync    = VSYNC_GPIO_NUM;
  g_config.pin_href     = HREF_GPIO_NUM;
  g_config.pin_sccb_sda = SIOD_GPIO_NUM;
  g_config.pin_sccb_scl = SIOC_GPIO_NUM;
  g_config.pin_pwdn     = PWDN_GPIO_NUM;
  g_config.pin_reset    = RESET_GPIO_NUM;

  // Niższy XCLK = mniejszy pobór prądu i większa stabilność na słabym zasilaniu
  g_config.xclk_freq_hz = 10000000;
  g_config.pixel_format = PIXFORMAT_JPEG;

  // Małe ramki + 1 bufor — minimalne zużycie RAM i prądu
  if (psramFound()) {
    g_config.frame_size   = FRAMESIZE_VGA;
    g_config.jpeg_quality = 12;
    g_config.fb_count     = 2;
    g_config.fb_location  = CAMERA_FB_IN_PSRAM;
    g_config.grab_mode    = CAMERA_GRAB_LATEST;
  } else {
    g_config.frame_size   = FRAMESIZE_QVGA;
    g_config.jpeg_quality = 15;
    g_config.fb_count     = 1;
    g_config.fb_location  = CAMERA_FB_IN_DRAM;
    g_config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
  }
}

static void powerCycle() {
  if (PWDN_GPIO_NUM >= 0) {
    pinMode(PWDN_GPIO_NUM, OUTPUT);
    digitalWrite(PWDN_GPIO_NUM, HIGH); // sleep
    delay(50);
    digitalWrite(PWDN_GPIO_NUM, LOW);  // wake
    delay(50);
  }
}

bool initCamera() {
  buildConfig();

  // Setup flash GPIO PRZED init kamery, żeby nie pływało
  ledcSetup(LEDC_FLASH_CH, 5000, 8);
  ledcAttachPin(4, LEDC_FLASH_CH);
  ledcWrite(LEDC_FLASH_CH, 0);

  for (int attempt = 1; attempt <= 5; attempt++) {
    powerCycle();
    esp_err_t err = esp_camera_init(&g_config);
    if (err == ESP_OK) {
      sensor_t *s = esp_camera_sensor_get();
      if (s) {
        Serial.printf("[CAM] Init OK (próba %d), PID=0x%02x\n", attempt, s->id.PID);
        if (s->id.PID == OV3660_PID) {
          s->set_vflip(s, 1);
          s->set_brightness(s, 1);
          s->set_saturation(s, -2);
        }
        s->set_framesize(s, psramFound() ? FRAMESIZE_VGA : FRAMESIZE_QVGA);
      }
      // Wygrzanie — wyrzuć pierwsze klatki które potrafią być puste
      for (int i = 0; i < 3; i++) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) esp_camera_fb_return(fb);
        delay(50);
      }
      return true;
    }
    Serial.printf("[CAM] Init fail 0x%x (próba %d/5)\n", err, attempt);
    esp_camera_deinit();
    delay(300);
  }

  Serial.println("[CAM] Init nieudany — restart za 2s");
  delay(2000);
  ESP.restart();
  return false;
}

bool reinitCamera() {
  Serial.println("[CAM] Recovery — reinicjalizacja kamery");
  esp_camera_deinit();
  delay(100);
  return initCamera();
}
