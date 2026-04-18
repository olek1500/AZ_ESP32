#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}
void loop() {
  goFoward(500,170);
  delay(500); 
}
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
void goFoward(int czas, int predkosc){
  ledcWrite( 2, predkosc);  //jazda
  ledcWrite(12, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void goBack(int czas, int predkosc){
  ledcWrite(14, predkosc);  //jazda
  ledcWrite(13, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void goRight(int czas, int predkosc){
  ledcWrite( 2, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void goLeft(int czas, int predkosc){
  ledcWrite(12, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void turnRight(int czas, int predkosc){
  ledcWrite( 2, predkosc);  //jazda
  ledcWrite(13, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void turnLeft(int czas, int predkosc){
  ledcWrite(12, predkosc);  //jazda
  ledcWrite(14, predkosc);  //jazda
  delay(czas);
  stopEngine();
}
void stopEngine() {
  ledcWrite( 2, 0);   //stop
  ledcWrite(12, 0);   //stop
  ledcWrite(13, 0);   //stop
  ledcWrite(14, 0);   //stop
}