#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}

void loop() {
  int licznik;
  for (licznik = 0; licznik < 4; licznik++) {
    goFoward(300, 255);
    delay(50);
    curveLeft(900, 20);
    delay(50);
  }
  for (licznik = 0; licznik < 4; licznik++) {
    goFoward(300, 255);
    delay(50);
    curveRight(900, 20);
    delay(50);
  }
}
