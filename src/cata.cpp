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
    
    case State::Matchload:
      cataMotor.move_velocity(90);

      if (cataFireState && cataMotor.get_efficiency() > 25) {
        cataFireState = false;
      }
      if (!cataFireState && cataMotor.get_efficiency() < 10) {
        cataFireState = true;
        matchloadCount++;
        if (matchloadCount >= 4) {
          leftDriveLED.set_all(0x990000);
          rightDriveLED.set_all(0x990000);
          frontLED.set_all(0x990000);
          backLED.set_all(0x990000);
        }
        if (matchloadCount >= 6) {
          cataState = State::Idle;
          lights.setColor(leftDriveLED);
          lights.setColor(rightDriveLED);
          lights.setColor(frontLED);
          lights.setColor(backLED);
          continue;
        }
      }
      break;
    
    case State::Single:
      cataMotor.move_velocity(83);      

      if (cataFireState && cataMotor.get_efficiency() > 25) {
        cataFireState = false;
      }
      if (!cataFireState && cataMotor.get_efficiency() < 10) {
        cataFireState = true;
        cataState = State::Idle;
      }
      break;

    case State::Idle:
      cataMotor.move(0);
      break;

    case State::Firing:
      cataMotor.move_velocity(80);      
      break;
    }
    pros::delay(15);

  }
}

void Catapult::setState(Catapult::State newState) {cataState = newState;};

Catapult::State Catapult::getState() { return cataState; };