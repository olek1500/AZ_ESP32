#include <Arduino.h>
#include "linia.h"

void setup() {
  initRobot();
  sensorRightLeftInit();
}
void loop() {
  int czujnikPrawy = sensorRight();
  int czujnikLewy  = sensorLeft();
  if((czujnikPrawy==LOW)&&(czujnikLewy==LOW)) {
    goFoward(50, 255);
  }
  else { 
    if((czujnikPrawy==HIGH)&&(czujnikLewy==LOW)) {
      turnRight(50, 255);
      goFoward(50, 255);
    }
    else {
      if((czujnikPrawy==LOW)&&(czujnikLewy==HIGH)) {
      turnLeft(50, 255);
      goFoward(50, 255);
      }
      else { delay(500); }
    }
  }
}