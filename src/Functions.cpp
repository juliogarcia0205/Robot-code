#include <Setup.h>

Joystick controller {0};
Talon left {0}, right {1};
DigitalInput limitSwitch {1};

float forwardVelocity, backwardsVelocity, turnVelocity;
double totalTurnVelocity, netLeft, netRight, speedMod = 1;
int gear = 3;

void updateRobot(int motorForwardButton, int motorBackwardButton, float speed, int forwardAxis, int backwardAxis, int turnAxis)
{
	updateGear();
	updateDriveTrain(forwardAxis, backwardAxis, turnAxis);

	if(controller.GetRawButton(motorForwardButton))
		elevatorSet(speed);
	else if(controller.GetRawButton(motorBackwardButton))
		elevatorSet(-speed);
	else
		elevatorStop();
}

void updateGear()
{
	if(controller.GetPOV() == 0) {
		gear = 3;
		speedMod = 1;
	}
	else if(controller.GetPOV() == 90) {
		gear = 2;
		speedMod = 0.5;
	}
	else if(controller.GetPOV() == 180){
		gear = 1;
		speedMod = 0.25;
	}
}

void updateDriveTrain(int forwardAxis, int backwardAxis, int turnAxis)
{
	forwardVelocity = controller.GetRawAxis(forwardAxis);
	backwardsVelocity = controller.GetRawAxis(backwardAxis);
	turnVelocity = controller.GetRawAxis(turnAxis);

	totalTurnVelocity = turnVelocity;// * 1.5;

	netLeft = ((forwardVelocity - backwardsVelocity) + totalTurnVelocity);
	netRight = ((backwardsVelocity - forwardVelocity) + totalTurnVelocity);

	left.Set(netLeft * speedMod);
	right.Set(netRight * speedMod);
}

void move(double speed)
{
	left.Set(speed);
	right.Set(speed);
}

void turnLeft(double speed)
{
	left.Set(-speed);
	right.Set(speed);
}

void turnRight(double speed)
{
	left.Set(speed);
	right.Set(-speed);
}

void stop() { move(0); }
