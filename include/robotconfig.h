#pragma once
#include "EZ-Template/drive/drive.hpp"
#include "Solenoid.hpp"
#include "cata.hpp"
#include "EZ-Template/util.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "main.h"
#include "lights.hpp"

extern Drive chassis;
extern pros::Controller controller;
extern balls::Catapult catapult;
extern Intake intake;
extern sylib::Addrled intakeLED;
extern sylib::Addrled doinkerLED;
extern ryan::Solenoid blocker;

extern ryan::Solenoid doinker;

extern std::shared_ptr<graphy::AsyncGrapher> velocityTime;

extern balls::Lights lights;