#pragma once
#include "TaskWrapper.hpp"
#include "main.h"
#include "pros/motors.hpp"

/**
 * @class Intake
 * @brief Represents an intake mechanism for a robot.
 * 
 * The Intake class provides functionality to control the state of the intake mechanism,
 * such as moving the intake in, holding it, moving it out, or keeping it idle.
 */
class Intake {

public:
  /**
   * @enum STATE
   * @brief Represents the possible states of the intake mechanism.
   */
  enum class STATE { IN, HOLD, OUT, IDLE, };

private:
  STATE state = STATE::IDLE; /**< The current state of the intake mechanism. */
  pros::MotorGroup &intakeMotors; /**< The motor used for the intake mechanism. */

public:
  /**
   * @brief Constructs an Intake object with the specified intake motor.
   * @param intakeMotor The motor used for the intake mechanism.
   */
  Intake(pros::MotorGroup &intakeMotors) : intakeMotors(intakeMotors) {}

  /**
   * @brief Gets the current state of the intake mechanism.
   * @return The current state of the intake mechanism.
   */
  STATE getState();

  /**
   * @brief Toggles the intake mechanism to the specified state.
   * @param newState The state to toggle the intake mechanism to.
   */
  void toggleTo(STATE newState);

  /**
   * @brief Updates the intake mechanism.
   * This method should be called periodically to update the state of the intake mechanism.
   */
  void update();

  /**
   * @brief Assigns the specified state to the intake mechanism.
   * @param newState The state to assign to the intake mechanism.
   */
  void operator=(STATE newState);
};