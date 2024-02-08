#pragma once
#include "main.h"
#include "pros/rotation.hpp"
#include "taskwrapper.hpp"
#include <memory>

namespace balls {
class Catapult : public ryan::TaskWrapper {

public:
  /**
   * @brief The State enum class represents the different states of an object.
   *        It can be either Idle, Firing, or Matchload.
   */
  enum class State { Idle, Firing, Matchload, Single };

private:
  State cataState;
  pros::Motor &cataMotor;

public:
  Catapult(pros::Motor &cataMotor) : cataMotor(cataMotor) {
    cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    cataState = State::Idle;
  }

  void loop() override;

  /**
   * @brief Sets the state of the catapult.
   *
   * This function is used to set the state of the catapult to a new state.
   *
   * @param newState The new state to set for the catapult.
   */
  void setState(Catapult::State newState);

  /**
   * Retrieves the state of the catapult.
   *
   * @return The state of the catapult.
   */
  Catapult::State getState();
};
} // namespace balls