#include "Solenoid.hpp"
#include "cata.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include <iostream>
#include "lemlib/api.hpp"

const int EXPANDER = 5;

sylib::Addrled underglowLED(EXPANDER, 8, 28);
sylib::Addrled intakeLED(EXPANDER, 7, 22);
sylib::Addrled rightDriveLED(22,6, 36);
sylib::Addrled leftDriveLED(22, 7, 36);
sylib::Addrled backLED(22, 8, 30);
//sylib::Addrled logo(EXPANDER, 8, 48);

pros::Motor leftFront(20, true);
pros::Motor leftBack(18, true);
pros::Motor leftTop(19, false);

pros::Motor rightFront(11, false);
pros::Motor rightBack(12, false);
pros::Motor rightTop(14, true);

pros::Motor_Group leftMotors({leftFront, leftBack, leftTop});
pros::Motor_Group rightMotors({rightFront, rightBack, rightTop});

pros::Imu imu(13);
// drivetrain
lemlib::Drivetrain drivetrain {&leftMotors, &rightMotors, 10.375, lemlib::Omniwheel::NEW_275, 450, 25};

// lateral motion controller
lemlib::ControllerSettings lateralController {12, 0, 30, 0, 1, 100, 3, 500, 3};

// angular motion controller
lemlib::ControllerSettings angularController {2, 0, 13.5, 0, 1, 100, 3, 500, 20};

// sensors for odometry
lemlib::OdomSensors sensors {nullptr, nullptr, nullptr, nullptr, &imu};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor cataMotor(21, false);

balls::Catapult catapult(cataMotor);

pros::Motor intakeMotor(9, true);

Intake intake(intakeMotor);

ryan::Solenoid blocker('D');

ryan::Solenoid Lwingus('C');
ryan::Solenoid Rwingus('E');

balls::Lights lights(underglowLED, intakeLED, backLED, leftDriveLED, rightDriveLED);