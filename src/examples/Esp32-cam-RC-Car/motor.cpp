#include "motor.h"

void motor_begin(void)
{
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);
}
void motor_forward(int speed)
{
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, LOW);
}

void motor_backward(int speed)
{
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, HIGH);
}

void motor_left(int speed)
{
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, LOW);
}

void motor_right(int speed)
{
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, HIGH);
}

void motor_stop(void)
{
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, LOW);
}
