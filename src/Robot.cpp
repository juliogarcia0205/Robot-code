#include <iostream>
#include <memory>
#include <string>
#include <Joystick.h>
#include <Talon.h>
#include <Spark.h>
#include <ctre/Phoenix.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <CameraServer.h>
#include <Timer.h>
#include <GripPipeline.h>
#define SRC_VISIONMETHODS_H_
using namespace frc;

class Robot: public frc::SampleRobot {
	Talon left {0}, right {1};
	Spark claw {2}, clawArm {3};
	Joystick controller {0}, joystick {1};
	SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoLeft = "Left Auto";
	const std::string autoCenter = "Center Auto";
	const std::string autoRight = "Right Auto";
	const std::string autoTest = "Test Auto";

	int gear = 3;
	double speedMod = 1;
	float xAxis = 0;
	float yAxis = 0;
	float zAxis = 0;

public:

	/* Code for Talon motor controllers

	TalonSRX * _tal = new TalonSRX(0);

	bool _btn5 = false;
	bool _currentLimEn = true;
	*/

	Robot() {
		//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
	}

	void RobotInit() {

		/* Code for Talon motor controllers

		const int kPeakCurrentAmps = 15;
		const int kPeakTimeMs = 0;
		const int kContinCurrentAmps = 10;

		_tal->ConfigPeakCurrentLimit(kPeakCurrentAmps, 10);
		_tal->ConfigPeakCurrentDuration(kPeakTimeMs, 10);
		_tal->ConfigContinuousCurrentLimit(kContinCurrentAmps, 10);
		_tal->EnableCurrentLimit(_currentLimEn);

		_tal->SetNeutralMode(NeutralMode::Brake);
		_tal->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
		_tal->SetSensorPhase(true);
		_tal->Config_kP(0, 2.0, 10);
		_tal->Config_kI(0, 0.0, 10);
		_tal->Config_kD(0, 0.0, 10);
		_tal->Config_kF(0, 0.0, 10);
		*/

		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoLeft, autoLeft);
		chooser.AddObject(autoCenter, autoCenter);
		chooser.AddObject(autoRight, autoRight);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		cs::UsbCamera camera1 = CameraServer::GetInstance()->StartAutomaticCapture();
		cs::UsbCamera camera2 = CameraServer::GetInstance()->StartAutomaticCapture();
		cs::CvSink GetVideo(const cs::VideoSource& camera);
	}

	void Autonomous() {
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoLeft) {
			std::cout << "Running Left Lane Autonomous" << std::endl;
			right.Set(-0.5);
			left.Set(0.5);
            Wait(2);
            right.Set(0);
            left.Set(0);
		}
		else if (autoSelected == autoCenter) {
			std::cout << "Running Center Lane Autonomous" << std::endl;
		}
		else if (autoSelected == autoRight) {
			std::cout << "Running Right Lane Autonomous" << std::endl;
			right.Set(-1);
			left.Set(1);
            Wait(1);
            right.Set(0);
            left.Set(0);
		}
		else {
			std::cout << "Running Default Autonomous" << std::endl;
		}
	}

	//Code run when operating

	void OperatorControl() override {
		while (IsOperatorControl() && IsEnabled()) {

			//Gear selector

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

			//Output speed and gear to SmartDashboard

			SmartDashboard::PutNumber("Speed Gear", gear);
			SmartDashboard::PutNumber("Speed", (left.GetSpeed() + (-right.GetSpeed()))/2);

			//Movement variables

			float forwardVelocity = controller.GetRawAxis(3);
			float backwardsVelocity = controller.GetRawAxis(2);
			float turnVelocity = controller.GetRawAxis(0);

			float forwardVelocityLeft = forwardVelocity;
			float backwardsVelocityLeft = backwardsVelocity;
			float forwardVelocityRight = forwardVelocity;
			float backwardsVelocityRight = backwardsVelocity;

			double totalTurnVelocity = turnVelocity;// * 1.5;

			double netLeft = ((forwardVelocityLeft - backwardsVelocityLeft) + totalTurnVelocity);
			double netRight = ((backwardsVelocityRight - forwardVelocityRight) + totalTurnVelocity);

			//Move main motors

			left.Set(netLeft * speedMod);
			right.Set(netRight * speedMod);

			//Claw arm control

			if (controller.GetRawButton(3) == 1){
				clawArm.Set(0.5);
			}
			else if(controller.GetRawButton(4) == 1){
				clawArm.Set(-0.5);
			}
			else {
				clawArm.Set(0);
			}

			//Claw control

			if (controller.GetRawButton(1) == 1) {
				claw.Set(0.5);
			} else if (controller.GetRawButton(2) == 1) {
				claw.Set(-0.5);
			} else {
				claw.Set(0);
			}

			/* Code for Talon motor controllers

			if (!_btn5 && controller.GetRawButton(1)) {
				//toggle current limit
				_currentLimEn = !_currentLimEn;
				//update Talon current limit
				_tal->EnableCurrentLimit(_currentLimEn);
				//print to DS
				printf("EnableCurrentLimit:%i\n", (int) _currentLimEn);
			}

			//save button state for next loop
			_btn5 = controller.GetRawButton(1);
			*/

			frc::Wait(0.005);
		}
	}

	//Runs during test mode

	void Test() override {

	}
};

START_ROBOT_CLASS(Robot)
