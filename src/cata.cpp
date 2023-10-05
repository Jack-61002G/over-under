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
      std::cout << "firing" << std::endl;
      
      if (pos < topTarget) { // fire until the cata is near it's top position
        cataState = State::Reloading;
      } else {
        cataMotor.move(110);
        break;
      }

    case State::Reloading:
      std::cout << "reloading" << std::endl;
      
      if ((pos >= bottomTarget) && (pos < 100)) { // reload until the cata is near it's bottom position
        cataState = State::Ready;
      } else {
        cataMotor.move(127);
        break;
      }

    case State::Ready:
      std::cout << "ready" << std::endl; // stop the cata
      cataMotor.move(0);
      break;
    }
    pros::delay(15);
  }
};

void Catapult::fire() {
  cataState = State::Firing;
  std::cout << "Fire Function" << std::endl;
}

void Catapult::changeTarget(double target) {
  bottomTarget = target;
  cataState = State::Reloading;
}