/*********
Parametry jak rotacja kamerki czy rozmieszczenie przyciskow
sa w pliku:     http.h
*********/

#include "../serwo.h"
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

bool detection_mode_enabled = false;
char detection_status[128] = "Wyłączony";
int target_x = -1;
int target_y = -1;
bool center_target_requested = false;
extern void motor_right(int speed);
extern void motor_stop(void);
extern void motor_forward(int speed);
extern void motor_left(int speed);

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  // Inicjalizacja serwomechanizmu na pinie 15
  initServo(15);

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
    config.fb_count = 2; // Zwiększenie ilości buforów do 2 zapobiega blokowaniu kamery przez telefon
  } else {
    Serial.printf("FRAMESIZE_QVGA (Brak PSRAM)");
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  s->set_whitebal(s, 1);
  s->set_awb_gain(s, 1);
  s->set_wb_mode(s, 0);
  s->set_saturation(s, 2);     // Sprzętowe podbicie nasycenia (+2). Czerwień będzie się mocniej odcinać od brązowej podłogi.

#if defined VFLIP_MIRROR
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
  if (detection_mode_enabled) {
    // Usunięte nadpisywanie statusu "Skanowanie otoczenia". 
    // Teraz odczyty barw (RGB) lub błędy utrzymają się na ekranie telefonu w trakcie trwania ruchu robota!

    // 2. Pobranie ramki (Kluczowa poprawka - odrzucamy starą klatkę z rozmytym obrazem ruchu)
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) esp_camera_fb_return(fb); 
    
    // Pobieramy świeżą klatkę po zatrzymaniu robota
    fb = esp_camera_fb_get();
    
    // Jeśli klatka jest pusta (np. zajęta przez strumień wideo), czekamy chwilkę i ponawiamy próbę
    if (!fb) {
      delay(100);
      fb = esp_camera_fb_get();
    }

    if (!fb) {
      Serial.println("Błąd: Nie udało się pobrać ramki z kamery");
      snprintf(detection_status, sizeof(detection_status), "Błąd: Brak ramki kamery!");
      delay(500); // Zatrzymanie na pół sekundy, aby telefon zdążył pobrać status błędu
      return;
    }

    // 3. Alokacja pamięci na obraz w formacie RGB565 (potrzebny do analizy pikseli)
    // QVGA ma wymiary 320x240. Każdy piksel zajmuje 2 bajty (RGB565).
    size_t rgb_buf_len = fb->width * fb->height * 2;
    uint8_t *rgb_buf = (uint8_t *)malloc(rgb_buf_len);
    
    if (rgb_buf == NULL) {
      Serial.println("Błąd: Brak pamięci RAM na konwersję RGB565!");
      snprintf(detection_status, sizeof(detection_status), "Błąd RAM! (Włącz PSRAM w Arduino)");
      esp_camera_fb_return(fb);
      delay(1000); // Zatrzymanie na sekundę, bo to krytyczny błąd
      return;
    }

    // 4. Konwersja JPEG -> RGB565
    if (!jpg2rgb565(fb->buf, fb->len, rgb_buf, JPG_SCALE_NONE)) {
      Serial.println("Błąd: Konwersja JPEG do RGB nie powiodła się");
      snprintf(detection_status, sizeof(detection_status), "Błąd konwersji obrazu!");
      free(rgb_buf);
      esp_camera_fb_return(fb);
      delay(500);
      return;
    }

    // Variables do obliczania środka ciężkości czerwonego obiektu
    uint32_t red_x_total = 0;
    uint32_t red_y_total = 0;
    uint32_t red_count = 0;
    
    // Zmienne diagnostyczne do debugowania widzianych kolorów
    int max_redness = -255;
    uint8_t best_r = 0, best_g = 0, best_b = 0;

// 5. Analiza pikseli obrazu RGB565
// POPRAWKA: Omijamy dolne 10 pikseli (y < fb->height - 10), aby pozbyć się fałszywych odczytów z uszkodzonego, pomarańczowego paska matrycy
    for (int y = 0; y < fb->height - 10; y++) {
      for (int x = 0; x < fb->width; x++) {
        // Poprawka KRYTYCZNA: Zły szyk bajtów w pamięci (Endianness)!
        // ESP32 to układ Little-Endian. Poprzednie ręczne składanie bajtów 
        // całkowicie niszczyło i obracało bity kolorów, przez co R mieszało się z G i B.
        uint16_t pixel = ((uint16_t *)rgb_buf)[y * fb->width + x];
        
        // Wyodrębnienie składowych R, G, B
        uint8_t r = ((pixel >> 11) & 0x1F) << 3;
        uint8_t g = ((pixel >> 5) & 0x3F) << 2;
        uint8_t b = (pixel & 0x1F) << 3;

        // "Czerwoność" - jak bardzo czerwony dominuje nad resztą
        int redness = (int)r - (g + b) / 2;
        if (redness > max_redness) {
          max_redness = redness;
          best_r = r; best_g = g; best_b = b;
        }

        // ULEPSZONY WARUNEK 2.0 (maksymalnie odporny na drewno i wady kamery):
        // 1. r > 50: Lekko podniesiony próg z 40, aby zignorować "szumy" całkowicie ciemnych obszarów.
        // 2. r > g * 2: Żądamy, by czerwieni było ponad DWA RAZY więcej niż zieleni! 
        //    Drewno to odcień pomarańczowego/brązowego (ma bardzo dużo zieleni np. R=120, G=70). 
        //    Dla drewna warunek (120 > 140) zwróci FAŁSZ, i drewno zostanie pominięte. Prawdziwy cel (np. R=160, G=40) przejdzie gładko!
        // 3. r * 10 > b * 15: Czerwień o min. 50% silniejsza od niebieskiego (zostawiamy drobną tolerancję na zimne światło jarzeniówek).
        if (r > 50 && (r > g * 2) && (r * 10 > b * 15)) {
          red_x_total += x;
          red_y_total += y;
          red_count++;
        }
      }
    }

    // 6. Decyzja na podstawie wyników detekcji
    // Próg podniesiony do 25 pikseli, aby wyeliminować pojedyncze "szumy" z matrycy i odblaski światła
    if (red_count > 25) { 
      uint16_t avg_x = red_x_total / red_count;
      uint16_t avg_y = red_y_total / red_count;
      
      target_x = avg_x;
      target_y = avg_y;
      Serial.printf("[Wykryto obiekt] X = %d, Y = %d, pikseli: %d\n", avg_x, avg_y, red_count);

      int16_t center_x = fb->width / 2; // Środek ekranu (dla QVGA to 160)
      int16_t offset = avg_x - center_x;
      
      if (center_target_requested) {
        // Wyznaczamy minimalne ruchy centrujące
        if (abs(offset) < 25) {
          Serial.println("Cel wyśrodkowany!");
          snprintf(detection_status, sizeof(detection_status), "Cel wyśrodkowany!");
          center_target_requested = false; // Koniec centrowania
        } 
        else if (offset > 0) {
          snprintf(detection_status, sizeof(detection_status), "Centrowanie w lewo...");
          motor_left(150);
          delay(25); // Minimalny krok
          motor_stop();
          delay(200); // Czas na ustabilizowanie obrazu po szarpnięciu
        } 
        else {
          snprintf(detection_status, sizeof(detection_status), "Centrowanie w prawo...");
          motor_right(150);
          delay(25); // Minimalny krok
          motor_stop();
          delay(200); // Czas na ustabilizowanie obrazu po szarpnięciu
        }
      } else {
        // Tryb stacjonarny - cel namierzony, aktualizujemy tylko celownik
        motor_stop();
        snprintf(detection_status, sizeof(detection_status), "Cel namierzony!");
        delay(50); // Krótkie odświeżanie, aby celownik był płynny
      }

    } else {
      target_x = -1;
      target_y = -1;
      center_target_requested = false; // Reset w przypadku zgubienia celu
      Serial.println("Brak czerwonego obiektu w tej klatce... Skanuję otoczenie.");
      snprintf(detection_status, sizeof(detection_status), "Szukam celu...");
      
      // --- Logika skanowania przeniesiona z góry pętli ---
      motor_right(130); // Zmniejszona prędkość szukania z 180 na 130 dla płynniejszego ruchu
      for(int i = 0; i < 10; i++) { // Skrócony czas impulsu (100ms zamiast 150ms) = mniejsze "kroki"
        if (!detection_mode_enabled) { motor_stop(); return; }
        delay(10);
      }
      motor_stop();
      
      // Zatrzymanie na chwilę, aby ustabilizować obraz przed zrobieniem zdjęcia w kolejnym kroku
      for(int i = 0; i < 30; i++) {
        if (!detection_mode_enabled) return;
        delay(10);
      }
    }

    // 7. Sprzątanie pamięci (Kluczowe, aby ESP32 się nie zawiesił po kilku klatkach!)
    free(rgb_buf);
    esp_camera_fb_return(fb);

  } else {
    target_x = -1;
    target_y = -1;
    center_target_requested = false;
    snprintf(detection_status, sizeof(detection_status), "Wyłączony");
    delay(50); // Tryb czuwania, gdy detekcja jest wyłączona
  }
}