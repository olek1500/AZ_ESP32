#include "osemki.h"

void initRobot() {
  ledcSetup(2, 5000, 8);
  ledcAttachPin(2, 2);
  ledcSetup(12, 5000, 8);
  ledcAttachPin(12, 12);
  ledcSetup(13, 5000, 8);
  ledcAttachPin(13, 13);
  ledcSetup(14, 5000, 8);
  ledcAttachPin(14, 14);
}
void stopEngine() {
  ledcWrite( 2, 0);
  ledcWrite(12, 0);
  ledcWrite(13, 0);
  ledcWrite(14, 0);
}
void goFoward(int czas, int predkosc) {
  ledcWrite( 2, predkosc);
  ledcWrite(12, predkosc);
  delay(czas);
  stopEngine();
}
void goBack(int czas, int predkosc) {
  ledcWrite(14, predkosc);
  ledcWrite(13, predkosc);
  delay(czas);
  stopEngine();
}
void goRight(int czas, int predkosc) {
  ledcWrite( 2, predkosc);
  delay(czas);
  stopEngine();
}
void goLeft(int czas, int predkosc) {
  ledcWrite(12, predkosc);
  delay(czas);
  stopEngine();
}
void turnRight(int czas, int predkosc) {
  ledcWrite( 2, predkosc);
  ledcWrite(13, predkosc);
  delay(czas);
  stopEngine();
}
void turnLeft(int czas, int predkosc) {
  ledcWrite(12, predkosc);
  ledcWrite(14, predkosc);
  delay(czas);
  stopEngine();
}
void curveRight(int czas, int turn) {
  ledcWrite( 2, 255);
  ledcWrite(12, 255 - turn);
  delay(czas);
  stopEngine();
}
void curveLeft(int czas, int turn) {
  ledcWrite(12, 255);
  ledcWrite( 2, 255 - turn);
  delay(czas);
  stopEngine();
}