#include "EZ-Template/api.hpp"
#include "Solenoid.hpp"
#include "intake.h"
#include "lemlib/api.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include <iostream>


// LED strips
sylib::Addrled backLED(22, 2, 36);
sylib::Addrled rightWingLED(22, 1, 18);
sylib::Addrled leftWingLED(22, 6, 18);
sylib::Addrled frontLED(22, 3, 36);
// lights object
balls::Lights lights(leftWingLED, rightWingLED, backLED, frontLED);



// Left drive motors
pros::Motor leftFront(11, true);
pros::Motor leftBack(4, true);
pros::Motor leftTop(1, true);
// Right drive motors
pros::Motor rightFront(20, false);
pros::Motor rightBack(10, false);
pros::Motor rightTop(8, false);
// Motor groups
pros::Motor_Group leftMotors({leftFront, leftBack, leftTop});
pros::Motor_Group rightMotors({rightFront, rightBack, rightTop});


// imu
pros::Imu imu(5);


// drivetrain
lemlib::Drivetrain drivetrain{&leftMotors, &rightMotors, 9.75, lemlib::Omniwheel::NEW_325, 450, 100};

// lateral motion controller
lemlib::ControllerSettings lateralController{15, 0, 30, 0, 1, 50, 3, 250, 10};

// angular motion controller
lemlib::ControllerSettings angularController(
  2,   // proportional gain (kP)
  0,   // integral gain (kI)
  10,  // derivative gain (kD)
  3,   // anti windup
  1,   // small error range, in degrees
  50,  // small error range timeout, in milliseconds
  3,   // large error range, in degrees
  250, // large error range timeout, in milliseconds
  0    // maximum acceleration (slew)
);


// sensors for odometry
lemlib::OdomSensors sensors{nullptr, nullptr, nullptr, nullptr, &imu};


// chassis
lemlib::Chassis lemChassis(drivetrain, lateralController, angularController, sensors);


//ez template drive
Drive chassis({-11, -4, -1}, {20, 10, 8}, 5, 3.25, 600, 1.333);


// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);


// intake motors
pros::Motor intake1(2, true);
pros::Motor intake2(9, false);
pros::Motor_Group intakeMotors({intake1, intake2});
Intake intake(intakeMotors);


// wing solenoids
ryan::Solenoid Lwingus('G');
ryan::Solenoid Rwingus('H');
