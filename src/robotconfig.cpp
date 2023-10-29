#include "Solenoid.hpp"
#include "cata.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "lights.hpp"
#include "main.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include <iostream>
#include "lemlib/api.hpp"

sylib::Addrled underglowLED(22, 1, 28);
sylib::Addrled intakeLED(22, 2, 22);
sylib::Addrled doinkerLED(22, 5, 32);

pros::Motor leftFront(11, true);
pros::Motor leftBack(20, true);
pros::Motor leftTop(10, false);

pros::Motor rightFront(8, false);
pros::Motor rightBack(7, false);
pros::Motor rightTop(2, true);

pros::Motor_Group leftMotors({leftFront, leftBack, leftTop});
pros::Motor_Group rightMotors({rightFront, rightBack, rightTop});

pros::Imu imu(9);
// drivetrain
lemlib::Drivetrain_t drivetrain {&leftMotors, &rightMotors, 10.375, lemlib::Omniwheel::NEW_325, 360, 13};

// lateral motion controller
lemlib::ChassisController_t lateralController {12, 30, 1, 100, 3, 500, 8};

// angular motion controller
lemlib::ChassisController_t angularController {2, 13.5, 1, 100, 3, 500, 20};

// sensors for odometry
lemlib::OdomSensors_t sensors {nullptr, nullptr, nullptr, nullptr, &imu};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

ryan::Solenoid doinker('C');

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor cataMotor(3, false);
pros::Rotation cataRotation(1, false);

balls::Catapult catapult(cataMotor, cataRotation, 57);

pros::Motor intakeMotor(19, true);

Intake intake(intakeMotor);

ryan::Solenoid blocker('D');

ryan::Solenoid hang('F');

balls::Lights lights(underglowLED, doinkerLED, intakeLED, doinker);