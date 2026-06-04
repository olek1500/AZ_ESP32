## Problem z ESP32 Arduino Core 1.0.6

Starsza wersja ESP32 Arduino Core (1.0.6) ma znane problemy z obsługą **automatycznego resetu** podczas wgrywania programu przez port USB/UART. Objawia się to tym, że:

- podczas uploadu płytka **nie przechodzi samoczynnie w tryb bootloadera**,
- narzędzie `esptool` zgłasza błąd typu `Failed to connect to ESP32: Timed out waiting for packet header`,
- konieczne jest **ręczne przytrzymanie przycisku RESET**, aby w ogóle rozpocząć wgrywanie.

## Instalacja nowego działającego core'a w Arduino IDE

Jeżeli upload działa tylko po ręcznym wciśnięciu BOOT, to **nie jest to wina płytki** — to ograniczenie starego core'a 1.0.6. Aktualizacja do **ESP32 Core 2.0.14** lub nowszego przywraca automatyczny reset i eliminuje problem z wgrywaniem programu.

Aby zainstalować nowy core w **Arduino IDE**, wykonaj następujące kroki:

1. Otwórz **Arduino IDE** i przejdź do `Plik → Preferencje` (ang. *File → Preferences*).
2. W polu **Dodatkowe adresy URL do menedżera płytek** (*Additional Boards Manager URLs*) wklej:

   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

   Jeżeli jest już tam inny adres, oddziel nowy przecinkiem lub dodaj w nowej linii.

3. Zatwierdź przyciskiem **OK**.
4. Przejdź do `Narzędzia → Płytka → Menedżer płytek...` (*Tools → Board → Boards Manager...*).
5. W wyszukiwarce wpisz **esp32**.
6. Znajdź pozycję **esp32 by Espressif Systems** i z listy rozwijanej wersji wybierz **2.0.14**.
7. Kliknij **Zainstaluj** i poczekaj na pobranie .
8. Po zakończeniu instalacji wybierz swoją płytkę w `Narzędzia → Płytka → esp32 → ESP32 Dev Module`.

## Weryfikacja

Po zainstalowaniu nowego core'a wystarczy podłączyć ESP32, wybrać właściwy port COM i kliknąć **Wgraj** — płytka powinna **sama wejść w tryb bootloadera** bez konieczności trzymania przycisku RESET.
 ## Ważna uwaga
 Wszystkie podzespoły tp. silnik, buzzer i dalekościomierze powinny być odłączone podczas uploadu. 