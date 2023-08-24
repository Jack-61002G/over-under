#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"

namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  enum class STATE { DOWN, HALF, RELOAD, RELOAD_HALF, FIRE, IDLE};

private:

  const double midTarget;
  const double bottomTarget;
  STATE state = STATE::IDLE;
  pros::Motor &cataMotor;
  pros::Rotation &cataRotation;

public:
  Catapult(pros::Motor &cataMotor,pros::Rotation &cataRotation , double midTarget, double bottomTarget)
      : cataMotor(cataMotor), cataRotation(cataRotation), midTarget(midTarget), bottomTarget(bottomTarget) {
    cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }

  STATE getState();

  void operator=(STATE newState);

  void setState(STATE newState);

  void loop() override;
};
} // namespace balls
