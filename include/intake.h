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

  STATE getState();
  void setState(STATE newState);
  void loop() override;
  void operator=(STATE newState);
};