#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}
void loop() {
  goFoward(300,255);
  delay(50);
  curveLeft(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveLeft(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveLeft(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveLeft(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveRight(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveRight(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveRight(900,20);
  delay(50);
  goFoward(300,255);
  delay(50);
  curveRight(900,20);
  delay(50);
}
