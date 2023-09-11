#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "chassis.hpp"
#include "intake.h"
#include "main.h"


extern balls::chassis chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;

extern sylib::Addrled intakeLED;

extern ryan::Solenoid doinker;
extern ryan::Solenoid LeftWingus;
extern ryan::Solenoid RightWingus;