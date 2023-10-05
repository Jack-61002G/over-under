#include "cata.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>

using namespace balls;

enum class State { Ready, Reloading, Firing };

void Catapult::loop() {
  State cataState = State::Reloading;

  while (true) {

    int pos = cataRotation.get_angle() / 100;
    std::cout << pos << std::endl;

    switch (cataState) {

    case State::Firing:

      if (cataRotation.get_velocity() / 100 < 200) {
        cataMotor.move(127);
        break;
      } else {
        cataState = State::Reloading;
        break;
      }

    case State::Reloading:
      std::cout << "reloading" << std::endl;
      if ((pos >= targetvalue) && (pos < 100)) {
        cataState = State::Ready;
      } else {
        cataMotor.move(127);
        break;
      }

    case State::Ready:
      std::cout << "ready" << std::endl;
      cataMotor.move(0);
      break;
    }
    pros::delay(15);
  }
};

void Catapult::fire() {
  cataState = State::Firing;
  std::cout << "FIRE" << std::endl;
}

void Catapult::changeTarget(double target) {
  targetvalue = target;
  cataState = State::Reloading;
}