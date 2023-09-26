#include "cata.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>

using namespace balls;

enum class State { Ready, Reloading, Firing };

int targetvalue;
State cataState = State::Reloading;

void Catapult::loop() {

  while (true) {

    int pos = cataRotation.get_angle() / 100;

    switch (cataState) {
    
    /*
    case State::Firing:
      cataMotor.move(110);
      while (pos > 30) {
        pros::delay(10);
      }
      cataMotor.move(0);
      pros::delay(750);
      cataState = State::Reloading;
      break;
    */
    
    case State::Firing:
      cataMotor.move(110);
      break;

    case State::Reloading:
      cataMotor.move(110);
      if (pos >= targetvalue)
        cataState = State::Ready;
      break;

    case State::Ready:
      cataMotor.move(0);
      break;
    }
    pros::delay(10);
  }
};

void Catapult::fire() {
  cataState = State::Firing;
  pros::delay(100);
  cataState = State::Reloading;
}