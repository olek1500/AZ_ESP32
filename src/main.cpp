#include <Arduino.h>

void setup() {
  pinMode( 2, OUTPUT); // lewy do przodu
  pinMode(12, OUTPUT); // prawy do przodu
  pinMode(13, OUTPUT); // prawy do tyłu
  pinMode(14, OUTPUT); // lewy do tyłu
}
void loop() {
  digitalWrite( 2, HIGH); // lewy do przodu ON
  digitalWrite(12, HIGH); // prawy do przodu ON
  digitalWrite(13, LOW);  // prawy do tyłu OFF 
  digitalWrite(14, LOW);  // lewy do tyłu OFF
  delay(250);             // czekaj 250ms
  digitalWrite( 2, LOW);  // lewy do przodu OFF
  digitalWrite(12, LOW);  // prawy do przodu OFF
  digitalWrite(13, LOW);  // prawy do tyłu OFF 
  digitalWrite(14, LOW);  // lewy do tyłu OFF  
  delay(500);             // czekaj pół sekundy
  digitalWrite( 2, LOW);  // lewy do przodu OFF
  digitalWrite(12, LOW);  // prawy do przodu OFF  
  digitalWrite(13, HIGH); // prawy do tyłu ON
  digitalWrite(14, HIGH); // lewy do tyłu ON
  delay(250);             // czekaj 250ms 
  digitalWrite( 2, LOW);  // lewy do przodu OFF
  digitalWrite(12, LOW);  // prawy do przodu OFF
  digitalWrite(13, LOW);  // prawy do tyłu OFF 
  digitalWrite(14, LOW);  // lewy do tyłu OFF  
  delay(500);
}