# MANIEK

Robot ESP32-CAM (4 silniki PWM przez H-bridge) z kamerą, własnym Wi-Fi (AP+STA) i panelem sterowania w przeglądarce. Repo wspiera dwa tryby:

- **Firmware kamery** (domyślny) — strumień MJPEG, panel webowy, REST do silników, zbieranie datasetu pod Teachable Machine. Płytka `esp32cam`.
- **Tryb edukacyjny** — przykłady jazdy bez kamery (ósemki, line follower) na bazie bibliotek `lib/`. Kompilowane przez podmianę `build_src_filter`, działa też w Arduino IDE.

## Struktura

```
src/main.cpp           — bootstrap firmware kamery (kamera → WiFi → serwer HTTP → silniki)
src/camera/            — inicjalizacja OV2640 z fallbackiem (5 prób + power-cycle)
src/wifi/              — łączenie WiFi w trybie AP+STA, retry, sygnalizacja LED-em flash
src/server/            — httpd na porcie 80 (UI + REST) i 81 (MJPEG stream)
include/ui/            — strony HTML osadzone w firmware (panel główny, dataset)
src/examples/          — szkice .cpp dla PlatformIO (bez kamery)
arduino/               — te same szkice w formacie .ino dla Arduino IDE
lib/                   — biblioteki edukacyjne (osemki, sterowanie, linia)
app/control.html       — samodzielna aplikacja sterowania robotem (otwierasz w przeglądarce)
docs/                  — szczegółowa dokumentacja
platformio.ini         — konfiguracja PlatformIO (board: esp32cam, PSRAM, src_filter)
```

## Szybki start (firmware kamery)

```bash
pio run -t upload          # kompilacja + wgranie
pio device monitor         # podgląd Serial
```

Po starcie ESP wypisuje w Serialu IP. Domyślny AP: SSID `<IP>_ESP32-CAM`, hasło `12345678`. SSID/hasło sieci STA są zaszyte w [src/wifi/wifi_manager.cpp](src/wifi/wifi_manager.cpp) (`Arduino` / `12345678`).

Otwórz w przeglądarce:

- `http://<IP>/` — panel główny z podglądem i ustawieniami kamery
- `http://<IP>/dataset` — narzędzie do zbierania zdjęć pod Teachable Machine
- `app/ai_sterowanie.html` (lokalnie) — D-pad + WASD + podgląd kamery, samodzielna aplikacja sterowania

Szczegóły:

- [docs/sterowanie.md](docs/sterowanie.md) — aplikacja `app/control.html` + REST API silników
- [docs/capture_dataset.md](docs/capture_dataset.md) — zbieranie datasetu
- [docs/examples.md](docs/examples.md) — tryb edukacyjny (biblioteki `lib/`, jazda bez kamery)
- [docs/problemy_z_uploadem.md](docs/problemy_z_uploadem.md) — fix dla starego ESP32 Core 1.0.6

## Pin-out

| GPIO | Funkcja | Kanał LEDC |
|------|---------|------------|
| 2    | PWM silnik LEWY — przód  | `LEDC_MOT1_CH` (1) |
| 14   | PWM silnik LEWY — tył    | `LEDC_MOT2_CH` (2) |
| 12   | PWM silnik PRAWY — przód | `LEDC_MOT3_CH` (3) |
| 13   | PWM silnik PRAWY — tył   | `LEDC_MOT4_CH` (5) |
| 4    | LED flash / PWM          | `LEDC_FLASH_CH` (4) |
| 0, 5, 18–27, 32–39 | piny kamery OV2640 — zarezerwowane | — |

GPIO 12 i 13 są strap-pinami — firmware trzyma je jako `INPUT` do końca boota i podpina PWM dopiero po inicjalizacji kamery i WiFi (zob. [src/main.cpp:13](src/main.cpp#L13)). Kanał LEDC 0 jest zarezerwowany przez driver kamery (XCLK 10 MHz).

## Tryb edukacyjny

Przykłady z `lib/` (osemki, line follower) napisane są pod zwykłą płytkę ESP32 (np. ESP32 Dev Module), bez kamery — używają tych samych pinów silników (2, 12, 13, 14) ale bez współdzielenia z kamerą i bez ograniczeń kanałów LEDC. Czujniki linii (`lib/linia`) na pinach 15 i 16 — uwaga, GPIO 16 koliduje z PSRAM na ESP32-CAM, więc na tej płytce line follower nie zadziała. Pełne instrukcje (Arduino IDE i PlatformIO): [docs/examples.md](docs/examples.md).
