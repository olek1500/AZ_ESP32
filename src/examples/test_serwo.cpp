#include <Arduino.h>
#include "serwo.h"

void setup() {
  // Inicjalizacja serwa na pinie D15
  initServo(15);
}

void loop() {
  // Płynny obrót od 0 do 180 stopni
  for (int kat = 0; kat <= 180; kat += 1) {
    setServoAngle(kat);
    delay(15); // Czekamy 15ms po każdym stopniu, żeby ruch był płynny
  }
  
  delay(500); // Chwila przerwy po dojechaniu na miejsce

  // Płynny powrót od 180 do 0 stopni
  for (int kat = 180; kat >= 0; kat -= 1) {
    setServoAngle(kat);
    delay(15);
  }
  
  delay(500); // Chwila przerwy przed kolejnym cyklem
}