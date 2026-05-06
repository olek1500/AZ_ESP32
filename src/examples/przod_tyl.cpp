#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}

void loop() {
  goFoward(400, 150); // Jedź do przodu przez 1 sekundę z pełną mocą
  delay(500);          // Odczekaj 0.5 sekundy w bezruchu
  goBack(400, 150);   // Jedź do tyłu przez 1 sekundę z pełną mocą
  delay(500);          // Odczekaj 0.5 sekundy w bezruchu
}