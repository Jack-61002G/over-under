#pragma once
#include "Solenoid.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "EZ-Template/api.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "autoSelect/selection.h"


extern lemlib::Chassis lemChassis;
extern Drive chassis;


extern pros::Controller controller;


extern Intake intake;


extern pros::IMU imu;


extern sylib::Addrled leftWingLED;
extern sylib::Addrled rightWingLED;
extern sylib::Addrled frontLED;
extern sylib::Addrled backLED;

extern balls::Lights lights;


extern pros::ADIEncoder enc;
extern lemlib::TrackingWheel tracking_wheel;


extern pros::Motor leftFront;
extern pros::Motor leftBack;
extern pros::Motor leftTop;
extern pros::Motor rightFront;
extern pros::Motor rightBack;
extern pros::Motor rightTop;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;


extern ryan::Solenoid Rwingus;
extern ryan::Solenoid Lwingus;
extern ryan::Solenoid doinker;
extern pros::ADIDigitalOut pto;
