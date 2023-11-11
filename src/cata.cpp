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

  int matchloadCount = 0;
  bool cataFireState = true;

  while (true) {
    int pos = cataRotation.get_angle() / 100;
    switch (cataState) {
    
    case State::Matchload:
      cataMotor.move(127);

      if (cataFireState && pos > 30 && pos < 100) {
        cataFireState = false;
      }
      if (!cataFireState && pos < 30) {
        cataFireState = true;
        matchloadCount++;
        if (matchloadCount >= 46) {
          cataState = State::Idle;
          continue;
        }
      }
      break;

    case State::Idle:
      cataMotor.move(0);
      break;

    case State::Firing:
      cataMotor.move(127);      
      break;
    }

    pros::delay(15);

  }
}

void Catapult::setState(Catapult::State newState) {cataState = newState;};

Catapult::State Catapult::getState() { return cataState; };