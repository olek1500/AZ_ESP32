#include "serwo.h"

Servo myServo; // Stworzenie obiektu serwa z nowej biblioteki

// Definicje funkcji do obsługi serwomechanizmu

void initServo(int pin) {
  myServo.setPeriodHertz(50);     // Standardowe serwomechanizmy pracują z częstotliwością 50 Hz
  myServo.attach(pin, 500, 2400); // Przypisanie serwa do pinu (wraz ze standardowymi wartościami impulsów)
}

void setServoAngle(int angle) {
  myServo.write(angle);           // Wysłanie konkretnego kąta (zazwyczaj między 0 a 180 stopni)
}