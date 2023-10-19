#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"
#include <memory>
namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  enum class State { Ready, Reloading, Firing };

private:
  State cataState;
  pros::Motor &cataMotor;
  pros::Rotation &cataRotation;
  int targetPos;

public:
  Catapult(pros::Motor &cataMotor, pros::Rotation &cataRotation, int bottomTarget)
    : cataMotor(cataMotor), cataRotation(cataRotation), bottomTarget(bottomTarget) 
    {
      cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
      cataState = State::Reloading;
    }

  void fire();

  void loop() override;

  void toggleTarget();
};
} // namespace balls