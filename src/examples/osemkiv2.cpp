#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}

void loop() {
  int licznik;
  for (licznik = 0; licznik < 4; licznik++) {
    goFoward(200, 255);
    delay(50);
    curveLeft(670, 100);
    delay(50);
  }
  for (licznik = 0; licznik < 4; licznik++) {
    goFoward(200, 255);
    delay(50);
    curveRight(670, 100);
    delay(50);
  }
}
