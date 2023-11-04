#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "lights.hpp"
#include "pros/motors.hpp"

extern lemlib::Chassis chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;
extern Intake intake;
extern sylib::Addrled intakeLED;
extern sylib::Addrled doinkerLED;
extern sylib::Addrled underglowLED;
extern ryan::Solenoid blocker;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern ryan::Solenoid doinker;

extern ryan::Solenoid hang;

extern std::shared_ptr<graphy::AsyncGrapher> velocityTime;

extern balls::Lights lights;