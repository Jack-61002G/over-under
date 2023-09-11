#include "cata.hpp"
#include "pros/misc.hpp"
#include <cmath>
using namespace balls;

Catapult::STATE Catapult::getState() { return this->state; }

void Catapult::setState(STATE newState) { this->state = newState; }

void Catapult::operator=(STATE newState) { this->state = newState; }

void Catapult::loop() {
  while (true) {
    switch (this->state) {
    case STATE::DOWN:
      cataMotor = 0;
      break;
    case STATE::HALF:
      cataMotor = 0;
      break;
    case STATE::RELOAD:
      if (cataRotation.get_position() < bottomTarget)
        cataMotor = cataVel->update(100, cataMotor.get_actual_velocity());

      else
        state = STATE::DOWN;
    case STATE::RELOAD_HALF:
      if ((cataRotation.get_position() < midTarget) ||
          (cataRotation.get_position() > midTarget + 5))
        cataMotor = cataVel->update(100, cataMotor.get_actual_velocity());

      else
        state = STATE::HALF;
      break;
    case STATE::FIRE:
      if (cataRotation.get_position() < midTarget) {
        this->state = STATE::RELOAD;
      } else {
        cataMotor = cataVel->update(100, cataMotor.get_actual_velocity());
      }
      break;
    case STATE::IDLE:
      if (cataRotation.get_position() < midTarget) {
        this->state = STATE::RELOAD_HALF;
      }

      break;
    }
  }
};