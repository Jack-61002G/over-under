#include "cata.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>

using namespace balls;

void Catapult::loop() {

  while (true) {

    int pos = cataRotation.get_angle() / 100;
    std::cout << pos << std::endl;

    switch (cataState) {

    case State::Firing:
      std::cout << "firing" << std::endl;
      cataMotor.move(127);
      break;

    case State::Reloading:
      std::cout << "reloading" << std::endl;
      cataMotor.move(127);
      if ((pos >= bottomTarget) && (pos < 100)) {
        cataState = State::Ready;
      } else {
        int power = 20 * (bottomTarget - pos);//cat
        cataMotor.move(127);
        break;
      }

    case State::Ready:
      std::cout << "ready" << std::endl; // stop the cata
      cataMotor.move(0);
      break;
    }
    pros::delay(5);

  } // while loop
};  // function

void Catapult::fire() {
  cataState = State::Firing;
  pros::delay(250);
  cataState = State::Reloading;
}

void Catapult::changeTarget(double target) {
  bottomTarget = target;
  cataState = State::Reloading;
}