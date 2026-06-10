#include <Arduino.h>
#include "linia.h"

void setup() {
  initRobot();
  sensorRightLeftInit();
}
void loop() {
  int czujnikPrawy = sensorRight();
  int czujnikLewy  = sensorLeft();
  if(czujnikPrawy==LOW) {
    if (czujnikLewy==HIGH) {
      turnRight(40, 200);
    }
    goFoward(6, 200);
  }
  else  {
    if (czujnikLewy==LOW) {
      turnLeft(40, 200);
      goFoward(6, 200);
    }
    else {
      delay(500);
    }
  }

}