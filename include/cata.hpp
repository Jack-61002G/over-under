#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"
#include "trapevel.h"
#include <memory>
namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  enum class STATE { Ready, Reloading, Firing };

private:
  STATE state;
  pros::Motor &cataMotor;
  pros::Rotation &cataRotation;
  std::shared_ptr<trapevel> cataVel =
      std::make_shared<trapevel>(1.5, 1.7, 4, 0.5);

public:
  Catapult(pros::Motor &cataMotor, pros::Rotation &cataRotation,
           double midTarget, double bottomTarget)
      : cataMotor(cataMotor), cataRotation(cataRotation) {
    cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }

  void fire();

  void loop() override;
};
} // namespace balls
