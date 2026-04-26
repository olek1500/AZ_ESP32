#include "server/cmd_handler.h"
#include "ledc_channels.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <esp32-hal-ledc.h>
#include <Arduino.h>

static String Feedback = "";
static String Command  = "";
static String cmd = "";
static String P1  = "", P2 = "", P3 = "", P4 = "", P5 = "";
static String P6  = "", P7 = "", P8 = "", P9 = "";

static byte ReceiveState   = 0;
static byte cmdState       = 1;
static byte strState       = 1;
static byte questionstate  = 0;
static byte equalstate     = 0;
static byte semicolonstate = 0;

void getCommand(char c) {
  if (c == '?') ReceiveState = 1;
  if ((c == ' ') || (c == '\r') || (c == '\n')) ReceiveState = 0;

  if (ReceiveState == 1) {
    Command = Command + String(c);
    if (c == '=') cmdState = 0;
    if (c == ';') strState++;

    if ((cmdState == 1) && ((c != '?') || (questionstate == 1)))              cmd = cmd + String(c);
    if ((cmdState == 0) && (strState == 1) && ((c != '=') || (equalstate == 1))) P1 = P1 + String(c);
    if ((cmdState == 0) && (strState == 2) && (c != ';')) P2 = P2 + String(c);
    if ((cmdState == 0) && (strState == 3) && (c != ';')) P3 = P3 + String(c);
    if ((cmdState == 0) && (strState == 4) && (c != ';')) P4 = P4 + String(c);
    if ((cmdState == 0) && (strState == 5) && (c != ';')) P5 = P5 + String(c);
    if ((cmdState == 0) && (strState == 6) && (c != ';')) P6 = P6 + String(c);
    if ((cmdState == 0) && (strState == 7) && (c != ';')) P7 = P7 + String(c);
    if ((cmdState == 0) && (strState == 8) && (c != ';')) P8 = P8 + String(c);
    if ((cmdState == 0) && (strState >= 9) && ((c != ';') || (semicolonstate == 1))) P9 = P9 + String(c);

    if (c == '?') questionstate = 1;
    if (c == '=') equalstate = 1;
    if ((strState >= 9) && (c == ';')) semicolonstate = 1;
  }
}

esp_err_t status_handler(httpd_req_t *req) {
  static char json_response[1024];
  sensor_t *s = esp_camera_sensor_get();
  char     *p = json_response;
  *p++ = '{';
  p += sprintf(p, "\"flash\":%d,",      0);
  p += sprintf(p, "\"framesize\":%u,",  s->status.framesize);
  p += sprintf(p, "\"quality\":%u,",    s->status.quality);
  p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
  p += sprintf(p, "\"contrast\":%d,",   s->status.contrast);
  p += sprintf(p, "\"hmirror\":%u,",    s->status.hmirror);
  p += sprintf(p, "\"vflip\":%u",       s->status.vflip);
  *p++ = '}'; *p++ = 0;

  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, strlen(json_response));
}

esp_err_t cmd_handler(httpd_req_t *req) {
  char   *buf;
  size_t  buf_len;
  char    variable[128] = {0,};
  char    value[128]    = {0,};
  String  myCmd = "";

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char *)malloc(buf_len);
    if (!buf) { httpd_resp_send_500(req); return ESP_FAIL; }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value,    sizeof(value))    == ESP_OK) {
      } else {
        myCmd = String(buf);
      }
    }
    free(buf); buf = NULL;
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  Feedback = ""; Command = "";
  cmd = ""; P1 = ""; P2 = ""; P3 = ""; P4 = ""; P5 = "";
  P6  = ""; P7 = ""; P8 = ""; P9 = "";
  ReceiveState = 0; cmdState = 1; strState = 1;
  questionstate = 0; equalstate = 0; semicolonstate = 0;

  if (myCmd.length() > 0) {
    myCmd = "?" + myCmd;
    for (int i = 0; i < (int)myCmd.length(); i++)
      getCommand(char(myCmd.charAt(i)));
  }

  if (cmd.length() > 0) {
    Serial.println("cmd= " + cmd + " ,P1= " + P1 + " ,P2= " + P2);

    if (P1 == "Class%201") {
      ledcWrite(LEDC_MOT1_CH, 0); ledcWrite(LEDC_MOT2_CH, 0);
      ledcWrite(LEDC_MOT3_CH, 0); ledcWrite(LEDC_MOT4_CH, 0);
    }
    if (P1 == "Class%202") {
      ledcWrite(LEDC_MOT1_CH, 750); ledcWrite(LEDC_MOT2_CH, 0);
      ledcWrite(LEDC_MOT3_CH, 650); ledcWrite(LEDC_MOT4_CH, 0);
    }
    if (P1 == "Class%203") {
      ledcWrite(LEDC_MOT1_CH, 0); ledcWrite(LEDC_MOT2_CH, 0);
      ledcWrite(LEDC_MOT3_CH, 650); ledcWrite(LEDC_MOT4_CH, 0);
    }

    if      (cmd == "your cmd") {}
    else if (cmd == "ip") {
      Feedback  = "AP IP: "  + WiFi.softAPIP().toString();
      Feedback += "<br>STA IP: " + WiFi.localIP().toString();
    }
    else if (cmd == "mac")          { Feedback = "STA MAC: " + WiFi.macAddress(); }
    else if (cmd == "restart")      { ESP.restart(); }
    else if (cmd == "digitalwrite") { ledcDetachPin(P1.toInt()); pinMode(P1.toInt(), OUTPUT); digitalWrite(P1.toInt(), P2.toInt()); }
    else if (cmd == "digitalread")  { Feedback = String(digitalRead(P1.toInt())); }
    else if (cmd == "analogwrite") {
      if (P1 == "4") {
        ledcSetup(LEDC_FLASH_CH, 5000, 8); ledcAttachPin(4, LEDC_FLASH_CH);
        ledcWrite(LEDC_FLASH_CH, P2.toInt());
      } else {
        ledcSetup(LEDC_AUX_CH, 5000, 8); ledcAttachPin(P1.toInt(), LEDC_AUX_CH);
        ledcWrite(LEDC_AUX_CH, P2.toInt());
      }
    }
    else if (cmd == "analogread")  { Feedback = String(analogRead(P1.toInt())); }
    else if (cmd == "touchread")   { Feedback = String(touchRead(P1.toInt())); }
    else if (cmd == "resetwifi") {
      for (int i = 0; i < 2; i++) {
        WiFi.begin(P1.c_str(), P2.c_str());
        long int startTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          if ((startTime + 5000) < millis()) break;
        }
        Feedback = "STAIP: " + WiFi.localIP().toString();
        if (WiFi.status() == WL_CONNECTED) {
          WiFi.softAP((WiFi.localIP().toString() + "_" + P1).c_str(), P2.c_str());
          for (int j = 0; j < 2; j++) {
            ledcWrite(LEDC_FLASH_CH, 10); delay(300);
            ledcWrite(LEDC_FLASH_CH, 0);  delay(300);
          }
          break;
        }
      }
    }
    else if (cmd == "flash")  { ledcWrite(LEDC_FLASH_CH, P1.toInt()); }
    else if (cmd == "serial") {
      if (P1 != "" && P1 != "stop") Serial.println(P1);
      if (P2 != "" && P2 != "stop") Serial.println(P2);
      Serial.println();
    }
    else { Feedback = "Command is not defined"; }

    if (Feedback == "") Feedback = Command;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, Feedback.c_str(), strlen(Feedback.c_str()));

  } else {
    int       val = atoi(value);
    sensor_t *s   = esp_camera_sensor_get();
    int       res = 0;

    if      (!strcmp(variable, "framesize"))  { if (s->pixformat == PIXFORMAT_JPEG) res = s->set_framesize(s, (framesize_t)val); }
    else if (!strcmp(variable, "quality"))    res = s->set_quality(s, val);
    else if (!strcmp(variable, "contrast"))   res = s->set_contrast(s, val);
    else if (!strcmp(variable, "brightness")) res = s->set_brightness(s, val);
    else if (!strcmp(variable, "hmirror"))    res = s->set_hmirror(s, val);
    else if (!strcmp(variable, "vflip"))      res = s->set_vflip(s, val);
    else if (!strcmp(variable, "flash"))      ledcWrite(LEDC_FLASH_CH, val);
    else                                      res = -1;

    if (res) return httpd_resp_send_500(req);
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, NULL, 0);
  }
}
