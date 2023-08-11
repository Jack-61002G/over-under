#include "TaskWrapper.hpp"
#include "main.h"

class Intake : public ryan::TaskWrapper {

public:
  enum class STATE { IN, OUT, IDLE, LOADED };

private:
  STATE state = STATE::IDLE;
  pros::Motor &intakeMotor;

public:
  Intake(pros::Motor &intakeMotor) : intakeMotor(intakeMotor) {
    this->startTask("Intake");
  }

  STATE getState() { return this->state; }
  void setState(STATE newState) { this->state = newState; }
  void loop() override {
    while (true) {

      state = (intakeMotor.get_efficiency() < 35) ? STATE::LOADED : state;

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
    }
  };
};