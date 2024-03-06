#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "autoSelect/selection.h"


extern lemlib::Chassis chassis;
extern pros::Controller controller;
extern pros::Motor cataMotor;
extern balls::Catapult catapult;
extern Intake intake;


extern sylib::Addrled leftDriveLED;
extern sylib::Addrled rightDriveLED;
extern sylib::Addrled frontLED;
extern sylib::Addrled backLED;


extern pros::ADIEncoder enc;
extern lemlib::TrackingWheel tracking_wheel;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern ryan::Solenoid Rwingus;
extern ryan::Solenoid Lwingus;
extern ryan::Solenoid doinker;
extern pros::ADIDigitalOut pto;

extern balls::Lights lights;

extern void intakeButtonTask_func();