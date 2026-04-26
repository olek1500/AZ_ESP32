#include "wifi/wifi_manager.h"
#include "ledc_channels.h"
#include <WiFi.h>
#include <esp32-hal-ledc.h>
#include <Arduino.h>

static const char *ssid       = "Arduino";
static const char *password   = "12345678";
static const char *apssid     = "ESP32-CAM";
static const char *appassword = "12345678";

void initWiFi() {
  WiFi.mode(WIFI_AP_STA);

  for (int i = 0; i < 2; i++) {
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    long int startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((startTime + 5000) < millis()) break;
    }

    if (WiFi.status() == WL_CONNECTED) {
      WiFi.softAP((WiFi.localIP().toString() + "_" + (String)apssid).c_str(), appassword);
      Serial.println("STAIP address: ");
      Serial.println(WiFi.localIP());

      for (int j = 0; j < 5; j++) {
        ledcWrite(LEDC_FLASH_CH, 10); delay(200);
        ledcWrite(LEDC_FLASH_CH, 0);  delay(200);
      }
      break;
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.softAP((WiFi.softAPIP().toString() + "_" + (String)apssid).c_str(), appassword);
    for (int i = 0; i < 2; i++) {
      ledcWrite(LEDC_FLASH_CH, 10); delay(1000);
      ledcWrite(LEDC_FLASH_CH, 0);  delay(1000);
    }
  }

  Serial.println("APIP address: ");
  Serial.println(WiFi.softAPIP());
}
