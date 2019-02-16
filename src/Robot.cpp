#include <Setup.h>

using namespace frc;

class Robot: public frc::SampleRobot
{
	Talon left {0}, right {1};

	SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoLeft = "Left Auto";
	const std::string autoCenter = "Center Auto";
	const std::string autoRight = "Right Auto";
	const std::string autoTest = "Test Auto";

public:

	Robot()
	{
		//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
	}

	void RobotInit()
	{
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoLeft, autoLeft);
		chooser.AddObject(autoCenter, autoCenter);
		chooser.AddObject(autoRight, autoRight);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		cs::UsbCamera camera1 = CameraServer::GetInstance()->StartAutomaticCapture();
		cs::UsbCamera camera2 = CameraServer::GetInstance()->StartAutomaticCapture();
		cs::CvSink GetVideo(const cs::VideoSource& camera);
	}

	void Autonomous()
	{
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoLeft) {
			std::cout << "Running Left Lane Autonomous" << std::endl;
			turnLeft(0.5);
            Wait(2);
            stop();
		}
		else if (autoSelected == autoCenter) {
			std::cout << "Running Center Lane Autonomous" << std::endl;
		}
		else if (autoSelected == autoRight) {
			std::cout << "Running Right Lane Autonomous" << std::endl;
			turnRight(1);
            Wait(1);
            stop();
		}
		else {
			std::cout << "Running Default Autonomous" << std::endl;
		}
	}

	void OperatorControl() override
	{
		while (IsOperatorControl() && IsEnabled())
		{
			updateRobot();

			SmartDashboard::PutNumber("Speed Gear", gear);
			SmartDashboard::PutNumber("Speed", (left.GetSpeed() + (-right.GetSpeed()))/2);

			frc::Wait(0.005);
		}
	}

	void Test() override
	{

	}
};

START_ROBOT_CLASS(Robot)
