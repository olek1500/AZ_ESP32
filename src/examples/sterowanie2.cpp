#include <Arduino.h>
#include "sterowanie.h"

void setup() {
  initRobot();
}
void loop() {
  goFoward(500,170);
  delay(500);
}
