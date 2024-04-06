#include "intake.h"
#include "main.h"

Intake::STATE Intake::getState() { return this->state; }

void Intake::toggleTo(STATE newState) {
  if (state != STATE::IDLE) {
    state = STATE::IDLE;
    update();
  } else {
    state = newState;
    update();
}}

void Intake::update() {
  switch (state) {
  case STATE::IN:
    intakeMotors.move(127);
    break;
  case STATE::HOLD:
    intakeMotors.move(20);
    break;
  case STATE::OUT:
    intakeMotors.move(-127);
    break;
  case STATE::SPIT:
    intakeMotors.move(-72);
    break;
  case STATE::IDLE:
    intakeMotors.move_velocity(0);
    break;
}}

void Intake::operator=(STATE newState) { 
  if (state != newState) { 
    state = newState; 
    update(); 
}}

