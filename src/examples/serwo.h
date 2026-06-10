#ifndef SERWO_H
#define SERWO_H

#include <Arduino.h>
#include <ESP32Servo.h>

// Deklaracje funkcji do obsługi serwomechanizmu
void initServo(int pin);
void setServoAngle(int angle);

#endif