#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "lights.hpp"

extern lemlib::Chassis chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;
extern Intake intake;
extern sylib::Addrled intakeLED;
extern sylib::Addrled doinkerLED;
extern ryan::Solenoid blocker;

extern ryan::Solenoid doinker;

extern ryan::Solenoid hang;

extern std::shared_ptr<graphy::AsyncGrapher> velocityTime;

extern balls::Lights lights;