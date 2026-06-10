# TRAKTOR — sterowanie robotem ESP32

Projekt PlatformIO (ESP32, framework Arduino). Robot napędzany przez 4 kanały PWM (`ledc*`) na pinach 2, 12, 13, 14. Opcjonalne czujniki linii na pinach 15, 16.

## Struktura

```
lib/
├── osemki/      — pełny zestaw funkcji ruchu (go*, turn*, curve*)
├── sterowanie/  — podzbiór bez funkcji curve*
└── linia/       — pełny ruch + czujniki linii (sensorRight/Left)
src/
├── main.cpp     — domyślny pusty szkic (kompilowany standardowo)
└── examples/    — gotowe szkice demonstracyjne (wykluczone z domyślnego buildu)
```

## Dostępne przykłady

| Plik | Biblioteka | Co robi |
|------|-----------|---------|
| `examples/osemkiv1.cpp` | `osemki` | Jazda ósemkami — sekwencja `goFoward` + `curveLeft`/`curveRight` |
| `examples/osemkiv2.cpp` | `osemki` | To samo co v1, ale z pętlą `for` |
| `examples/sterowanie.cpp` | `osemki` | Prosta sekwencja jazdy z łukami |
| `examples/sterowanie2.cpp` | `sterowanie` | Najprostsza jazda do przodu w pętli |
| `examples/line_follower1.cpp` | `linia` | Line follower — wersja z trzema warunkami `if` |
| `examples/line_follower2.cpp` | `linia` | Line follower — wersja zagnieżdżona, krótsze impulsy |

## Jak uruchomić wybrany przykład

Przykłady są domyślnie wykluczone z buildu przez `build_src_filter` w `platformio.ini`. Aby skompilować i wgrać wybrany szkic, w `platformio.ini` podmień linię:

```ini
build_src_filter = +<*> -<examples/>
```

na (wybierając jeden z plików):

```ini
build_src_filter = -<*> +<examples/osemkiv1.cpp>
```

Znak `-<*>` wyłącza `main.cpp` (inaczej dwa razy zdefiniowane `setup`/`loop`), `+<examples/...>` włącza konkretny przykład.

Następnie z katalogu projektu:

```bash
pio run -t upload          # kompilacja + wgranie na ESP32
pio device monitor          # podgląd Serial (115200)
```

Po skończonej zabawie z przykładem przywróć oryginalny filtr, żeby `main.cpp` znów był aktywny:

```ini
build_src_filter = +<*> -<examples/>
```

## Pin-out

| Pin | Funkcja |
|-----|---------|
| 2   | PWM silnik — przód/prawo |
| 12  | PWM silnik — przód/lewo |
| 13  | PWM silnik — tył/prawo |
| 14  | PWM silnik — tył/lewo |
| 15  | Czujnik linii — prawy (`linia`) |
| 16  | Czujnik linii — lewy (`linia`) |

## Pisanie własnego szkicu

Stwórz nowy plik w `src/examples/<nazwa>.cpp`:

```cpp
#include <Arduino.h>
#include "osemki.h"      // lub "sterowanie.h" / "linia.h"

void setup() {
  initRobot();
}

void loop() {
  goFoward(500, 200);
  delay(300);
  curveLeft(400, 30);
}
```

Aktywuj go w `platformio.ini` tak jak gotowe przykłady wyżej.

## Problemy z uploadem

Jeśli ESP32 nie wchodzi sam w tryb bootloadera podczas wgrywania, zobacz `Problemy/problemy_z_uploadem.md` (znana sprawa w starym ESP32 Arduino Core 1.0.6).
