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
    
    case State::Firing:
      cataMotor.move(110);
      break;

    case State::Reloading:
      if (pos >= targetvalue) { cataState = State::Ready; }
      else { cataMotor.move(110); break; }

    case State::Ready:
      cataMotor.move(0);
      break;

    }
    pros::delay(10);
  }
};

void Catapult::fire() {
  cataState = State::Firing;
  pros::delay(100);            // mess with this when tuning the cata
  cataState = State::Reloading;
}

void Catapult::changeTarget(double target) {
  targetvalue = target; 
  cataState = State::Reloading;
}