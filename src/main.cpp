#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "camera/camera_init.h"
#include "wifi/wifi_manager.h"
#include "server/http_server.h"
#include "ledc_channels.h"
#include <esp32-hal-ledc.h>
#include <Arduino.h>

static void initMotorPins() {
  // Trzymaj GPIO 12/13 jako wejście podczas boota (strap pins),
  // PWM podpinaj DOPIERO po inicjalizacji kamery i WiFi
  pinMode(12, INPUT);
  pinMode(13, INPUT);
}

static void attachMotors() {
  ledcSetup(LEDC_MOT1_CH, 1000, 10); ledcAttachPin(2,  LEDC_MOT1_CH); ledcWrite(LEDC_MOT1_CH, 0);
  ledcSetup(LEDC_MOT2_CH, 1000, 10); ledcAttachPin(14, LEDC_MOT2_CH); ledcWrite(LEDC_MOT2_CH, 0);
  ledcSetup(LEDC_MOT3_CH, 1000, 10); ledcAttachPin(12, LEDC_MOT3_CH); ledcWrite(LEDC_MOT3_CH, 0);
  ledcSetup(LEDC_MOT4_CH, 1000, 10); ledcAttachPin(13, LEDC_MOT4_CH); ledcWrite(LEDC_MOT4_CH, 0);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  initMotorPins();

  if (!initCamera()) {
    Serial.println("[MAIN] Kamera niedostępna — kontynuuję bez niej");
  }

  initWiFi();
  startCameraServer();

  attachMotors();
  ledcWrite(LEDC_FLASH_CH, 0);
}

void loop() {
  delay(1000);
}
