#include "Solenoid.hpp"
#include "cata.hpp"
#include "intake.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include <iostream>
#include "lemlib/api.hpp"

sylib::Addrled underglowLED(22, 5, 31);
sylib::Addrled rightDriveLED(22,2, 36);
sylib::Addrled leftDriveLED(12, 6, 36);
sylib::Addrled backLED(12, 7, 28);
//sylib::Addrled logo(EXPANDER, 8, 48);

pros::Motor leftFront(8, true);
pros::Motor leftBack(10, true);
pros::Motor leftTop(9, false);

pros::Motor rightFront(4, false);
pros::Motor rightBack(11, false);
pros::Motor rightTop(1, true);

pros::Motor_Group leftMotors({leftFront, leftBack, leftTop});
pros::Motor_Group rightMotors({rightFront, rightBack, rightTop});

pros::Imu imu(7);
// drivetrain
lemlib::Drivetrain drivetrain {&leftMotors, &rightMotors, 9.75, lemlib::Omniwheel::NEW_275, 450, 100};

// lateral motion controller
lemlib::ControllerSettings lateralController {15, 0, 30, 0, 1, 100, 3, 500, 10};

// angular motion controller
lemlib::ControllerSettings angularController {2.3, 0, 13.5, 0, 1, 100, 3, 500, 20};

// sensors for odometry
lemlib::OdomSensors sensors {nullptr, nullptr, nullptr, nullptr, &imu};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor cataMotor(3, false);

balls::Catapult catapult(cataMotor);

pros::Motor intakeMotor(20, true);

Intake intake(intakeMotor);

ryan::Solenoid doinker('G');

ryan::Solenoid Lwingus('H');
ryan::Solenoid Rwingus('F');
pros::ADIDigitalOut pto ({{12, 'E'}});

balls::Lights lights(underglowLED, backLED, leftDriveLED, rightDriveLED, Lwingus, Rwingus, intake);