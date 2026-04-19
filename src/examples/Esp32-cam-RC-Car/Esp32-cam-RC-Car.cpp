/*********
Parametry jak rotacja kamerki czy rozmieszczenie przyciskow
sa w pliku:     http.h
*********/

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems
#include "esp_http_server.h"
#include "camera.h"


#define USE_AP_MODE //if you don't have a router
#define VFLIP_MIRROR // if your camera shows fliped image1q

#if defined USE_AP_MODE
const char *soft_ap_ssid = "SIC_Robot_SHARKY";
//const char *soft_ap_password = "12345678";
#else
//const char* wifi_network_ssid = "KT_GiGA_7EA2";
//const char* wifi_network_password =  "7bdc00fi91";
//const char* wifi_network_ssid = "U+Net900F";
//const char* wifi_network_password =  "908F7F99M!";
#endif
 
void startCameraServer();
void motor_begin(void);

void setup() {
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  motor_begin();

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    Serial.printf("FRAMESIZE_QVGA");
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    Serial.printf("FRAMESIZE_SVGA");
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
#if defined VFLIP_MIRROR
  sensor_t * s = esp_camera_sensor_get();
  s->set_hmirror(s, 1);        // 0 = disable , 1 = enable
  s->set_vflip(s, 1);          // 0 = disable , 1 = enable
#endif

#if defined USE_AP_MODE
  WiFi.softAP(soft_ap_ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
#else
  // Wi-FI connect
  WiFi.begin(wifi_network_ssid, wifi_network_password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected : ");
  Serial.println(WiFi.localIP());
#endif

  // Start streaming web server
  startCameraServer();
}

void loop() {
}
