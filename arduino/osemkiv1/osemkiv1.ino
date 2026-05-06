#include <Arduino.h>
#include "osemki.h"

void setup() {
  initRobot();
}
void loop() {
  // Ciągła jazda po łuku w lewo przez około 3 sekundy, aby zatoczyć pełne koło
  curveLeft(3000, 150);
  delay(100); 

  // Ciągła jazda po łuku w prawo przez około 3 sekundy, aby zatoczyć drugie koło
  curveRight(3000, 150);
  delay(100);
}
