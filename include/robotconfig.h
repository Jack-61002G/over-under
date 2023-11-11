#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "lights.hpp"
#include "main.h"
#include "pros/motors.hpp"
#include "trapevel.h"


extern lemlib::Chassis chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;
extern Intake intake;
extern sylib::Addrled intakeLED;
extern sylib::Addrled doinkerLED;
extern sylib::Addrled underglowLED;
extern sylib::Addrled rightDriveLED;
extern sylib::Addrled leftDriveLED;
extern ryan::Solenoid blocker;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern ryan::Solenoid doinker;

extern ryan::Solenoid hang;

extern std::shared_ptr<graphy::AsyncGrapher> velocityTime;

extern balls::Lights lights;

extern std::unique_ptr<trapevel> leftController;
extern std::unique_ptr<trapevel> rightController;