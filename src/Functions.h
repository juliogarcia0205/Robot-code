#ifndef SRC_FUNCTIONS_H_
#define SRC_FUNCTIONS_H_

#include <Setup.h>

void updateRobot(int motorForwardButton = 0, int motorBackwardButton = 1, float speed = 1, int forwardAxis = 3, int backwardAxis = 2, int turnAxis = 0);
void updateGear();
void updateDriveTrain(int forwardAxis = 3, int backwardAxis = 2, int turnAxis = 0);
void move(double speed);
void turnLeft(double speed);
void turnRight(double speed);
void stop();

#endif
