#pragma once
#include "Solenoid.hpp"
#include "cata.hpp"
#include "chassis.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "main.h"

extern balls::chassis chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;
extern Intake intake;
extern sylib::Addrled intakeLED;
extern sylib::Addrled LwingusLED;
extern sylib::Addrled RwingusLED;
extern sylib::Addrled deadassLogo;

extern ryan::Solenoid doinker;
extern ryan::Solenoid LeftWingus;
extern ryan::Solenoid RightWingus;

extern std::shared_ptr<graphy::AsyncGrapher> velocityTime;