#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "camera/camera_init.h"
#include "wifi/wifi_manager.h"
#include "server/http_server.h"
#include "ledc_channels.h"
#include <esp32-hal-ledc.h>
#include <Arduino.h>

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  initCamera();
  initWiFi();
  startCameraServer();

  ledcSetup(LEDC_MOT1_CH, 1000, 10); ledcAttachPin(2,  LEDC_MOT1_CH);
  ledcSetup(LEDC_MOT2_CH, 1000, 10); ledcAttachPin(14, LEDC_MOT2_CH);
  ledcSetup(LEDC_MOT3_CH, 1000, 10); ledcAttachPin(12, LEDC_MOT3_CH);
  ledcSetup(LEDC_MOT4_CH, 1000, 10); ledcAttachPin(13, LEDC_MOT4_CH);

  digitalWrite(4, LOW);
}

void loop() {
}
