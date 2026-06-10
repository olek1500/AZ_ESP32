# TRAKTOR

Sterowanie robotem ESP32 (4 silniki PWM, opcjonalnie czujniki linii). Repo wspiera **PlatformIO** i **Arduino IDE** równolegle.

## Struktura

```
lib/                — biblioteki (osemki, sterowanie, linia) — wspólne dla obu IDE
src/main.cpp        — domyślny pusty szkic PlatformIO
src/examples/       — szkice .cpp dla PlatformIO
arduino/            — te same szkice w formacie .ino dla Arduino IDE
docs/               — szczegółowa dokumentacja
platformio.ini      — konfiguracja PlatformIO
```

Szczegóły bibliotek, pin-out i opisy przykładów: zobacz [`docs/examples.md`](docs/examples.md).

## Lista przykładów

| Szkic | Biblioteka | Co robi |
|-------|------------|---------|
| `osemkiv1` | `osemki` | Jazda ósemkami (sekwencja prosta) |
| `osemkiv2` | `osemki` | Jazda ósemkami (z pętlą `for`) |
| `sterowanie` | `osemki` | Prosta sekwencja jazdy z łukami |
| `sterowanie2` | `sterowanie` | Najprostsza jazda do przodu |
| `line_follower1` | `linia` | Line follower — trzy warunki `if` |
| `line_follower2` | `linia` | Line follower — wersja zagnieżdżona |

---

## Uruchomienie w Arduino IDE

### 1. Jednorazowa instalacja bibliotek

Skopiuj foldery z `lib/` do katalogu bibliotek Arduino (`...\Arduino\libraries\` na Windows):

### 2. Otwarcie szkicu

`File → Open…` → wybierz np. `arduino/osemkiv1/osemkiv1.ino`.

### 3. Konfiguracja płytki

- `Tools → Board → ESP32 → ESP32 Dev Module`
- `Tools → Port` → port ESP32
- `Upload` (Ctrl+U)

### Brak wsparcia ESP32 w Arduino IDE?

Pełna instrukcja instalacji core'a 2.0.14: [`docs/problemy_z_uploadem.md`](docs/problemy_z_uploadem.md). 
---

## Pisanie własnego szkicu

**PlatformIO** — nowy plik w `src/examples/<nazwa>.cpp`:

Aktywuj zmieniając `build_src_filter`.

**Arduino IDE** — utwórz folder `arduino/<nazwa>/` z plikiem `<nazwa>.ino` o tej samej treści.
