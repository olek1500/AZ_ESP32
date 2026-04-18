#pragma once
#include <Arduino.h>

void initRobot();
void stopEngine();
void goFoward(int czas, int predkosc);
void goBack(int czas, int predkosc);
void goRight(int czas, int predkosc);
void goLeft(int czas, int predkosc);
void turnRight(int czas, int predkosc);
void turnLeft(int czas, int predkosc);
void curveRight(int czas, int turn);
void curveLeft(int czas, int turn);

void sensorRightLeftInit();
int sensorRight();
int sensorLeft();
