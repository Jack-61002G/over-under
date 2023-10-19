#include "intake.h"
#include "main.h"

Intake::STATE Intake::getState() { return this->state; }

void Intake::setState(STATE newState) { this->state = newState; }

void Intake::operator=(STATE newState) { this->state = newState; }

void Intake::loop() {
  while (true) {
    switch (this->state) {
    case STATE::IN:
      intakeMotor = 127;
      break;
    case STATE::OUT:
      intakeMotor = -127;
      break;
    case STATE::IDLE:
      intakeMotor = 0;
      break;
    case STATE::LOADED:
      intakeMotor = 0;
      break;
    }
    pros::delay(20);
  }
};