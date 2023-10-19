#include "cata.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>
#include <ostream>

using namespace balls;

void Catapult::loop() {

  while (true) {
    int pos = cataRotation.get_angle() / 100;

    switch (cataState) {

    case State::Firing:
      cataMotor.move(127);
      break;

    case State::Reloading:
      if ((pos >= targetPos) && (pos < 100)) {
        cataState = State::Ready;
      } else { int power = 20 * (targetPos - pos); // I don't know what this does but I don't want to remove it
        cataMotor.move(127);
        break;
      }

    case State::Ready:
      cataMotor.move(0);
      break;
    }

    pros::delay(10);

  } // while loop
};  // function

void Catapult::fire() {
  cataState = State::Firing;
  pros::delay(250);
  cataState = State::Reloading;
}