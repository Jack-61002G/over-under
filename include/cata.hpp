#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"
#include "trapevel.h"
#include <memory>
namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  enum class State { Ready, Reloading, Firing };

private:
  State cataState;
  pros::Motor &cataMotor;
  pros::Rotation &cataRotation;
  int topTarget;
  int bottomTarget;

public:
  Catapult(pros::Motor &cataMotor, pros::Rotation &cataRotation, int targetvalue) 
  : cataMotor(cataMotor), cataRotation(cataRotation), topTarget(topTarget), bottomTarget(bottomTarget) {
    cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }

  void fire();

  void loop() override;

  void changeTarget(double target);
};
} // namespace balls