#include "wifi/wifi_manager.h"
#include "ledc_channels.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include <esp32-hal-ledc.h>
#include <Arduino.h>

static const char *ssid       = "Arduino";
static const char *password   = "12345678";
static const char *apssid     = "ESP32-CAM";
static const char *appassword = "12345678";

static constexpr uint32_t STA_TIMEOUT_MS = 5000;
static constexpr uint8_t  STA_RETRIES    = 2;

static void blinkFlash(uint8_t times, uint16_t onMs, uint16_t offMs) {
  for (uint8_t i = 0; i < times; i++) {
    ledcWrite(LEDC_FLASH_CH, 10); delay(onMs);
    ledcWrite(LEDC_FLASH_CH, 0);  delay(offMs);
  }
}

static String buildApSsid(const IPAddress &ip) {
  String s = ip.toString() + "_" + apssid;
  if (s.length() > 32) s = s.substring(0, 32);
  return s;
}

static void startAp(const IPAddress &ip) {
  WiFi.softAP(buildApSsid(ip).c_str(), appassword);
  Serial.print("APIP address: ");
  Serial.println(WiFi.softAPIP());
}

static bool tryConnectSta() {
  for (uint8_t attempt = 1; attempt <= STA_RETRIES; attempt++) {
    Serial.printf("[WiFi] Próba %u/%u łączenia z %s\n", attempt, STA_RETRIES, ssid);

    WiFi.disconnect(true, true);
    delay(100);
    WiFi.begin(ssid, password);

    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < STA_TIMEOUT_MS) {
      delay(250);
      esp_task_wdt_reset();
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[WiFi] STA połączone, IP: ");
      Serial.println(WiFi.localIP());
      return true;
    }

    Serial.println("[WiFi] Timeout — kolejna próba");
  }
  return false;
}

void initWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleep(false);
  esp_wifi_set_max_tx_power(34); // ~8.5 dBm — mniejszy peak prądu

  // AP startuje od razu — żeby urządzenie było dostępne nawet gdy STA fail
  startAp(WiFi.softAPIP());

  if (tryConnectSta()) {
    // Restart AP z SSID zawierającym IP STA (po połączeniu znamy adres)
    startAp(WiFi.localIP());
    blinkFlash(5, 200, 200);
  } else {
    Serial.println("[WiFi] STA nieudane — tryb AP-only");
    blinkFlash(2, 1000, 1000);
  }
}
