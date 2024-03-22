#include "main.h"
#include "autoSelect/selection.h"
#include "autons.hpp"
#include "cata.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"

void initialize() {

  // Initialize chassis and auton selector
  sylib::initialize();
  
  pros::delay(500);

  selector::init();

  chassis.initialize();
  lemChassis.calibrate(false);

  lights.startTask();
  pros::Task intakeButtonTask(intakeButtonTask_func);

  garage_constants(); // Set the drive to your own constants from autons.cpp!
  exit_condition_defaults(); // Set the exit conditions to your own constants
                             // from autons.cpp!
  selector::init();
}

void autonomous() {
  lights.auton = selector::auton;

  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);



  if (std::abs(selector::auton) == 1) {
    return;
  } else if (std::abs(selector::auton) == 2) {
    return;
  } else if (std::abs(selector::auton) == 3) {
    return;
  } else if (std::abs(selector::auton) == 4) {
    return;
  } else if (selector::auton == 0) {
    return;
  }
}

void disabled() {
  lights.auton = selector::auton;

  Lwingus.set(false);
  Rwingus.set(false);

  // Store the time at the start of the loop
  std::uint32_t clock = sylib::millis();
  while (true) {
    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

void opcontrol() {

  std::uint32_t clock = sylib::millis();

  lights.auton = selector::auton;

  while (true) {

    // chassis control
    int turnVal = lemlib::defaultDriveCurve(
        controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 4);
    int latVal = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    if (std::abs(latVal) > 114 && std::abs(turnVal) > 114) {
      if (latVal < 0) {
        latVal = -95;
      } else {
        latVal = 95;
      }
    }
    lemChassis.arcade(latVal, turnVal);

    // std::cout << cataMotor.get_efficiency() << std::endl;

    // intake control
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake = Intake::STATE::IN;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake = Intake::STATE::OUT;
    } else {
      intake = Intake::STATE::IDLE;
    }

    // pistons

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      Rwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      Lwingus.toggle();
    }

    sylib::delay_until(&clock, 10);
  }
}