#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"
#include <memory>
namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  enum class State { Ready, Reloading, Firing, MatchloadSkills};

private:
  State cataState;
  pros::Motor &cataMotor;
  pros::Rotation &cataRotation;
  int targetPos;

public:
  Catapult(pros::Motor &cataMotor, pros::Rotation &cataRotation, int targetPos)
    : cataMotor(cataMotor), cataRotation(cataRotation), targetPos(targetPos) 
    {
      cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
      cataState = State::Ready;
    }

  void loop() override;

  void fire();
  
};
} // namespace balls