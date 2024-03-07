#include "cata.hpp"
#include "autoSelect/selection.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>
#include <ostream>

using namespace balls;

void Catapult::loop() {

  int matchloadCount = 0;
  bool cataFireState = true;
  cataMotor.set_gearing(pros::E_MOTOR_GEARSET_36);

  while (true) {
    switch (cataState) {

    case State::Idle:
      cataMotor = 0;
      break;

    case State::Firing:
      cataMotor = 127;
      break;
    }
    pros::delay(15);

  }
}

void Catapult::setState(Catapult::State newState) {cataState = newState;};

Catapult::State Catapult::getState() { return cataState; };