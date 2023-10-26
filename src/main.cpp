#include "main.h"
#include "EZ-Template/auton.hpp"
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "gif-pros/gifclass.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "robotconfig.h"
#include "sylib/system.hpp"

ASSET(cat_gif)

void initialize() {
  // Print our branding over your terminal :D
  ez::print_ez_template();

  pros::delay(
      500); // Stop the user from doing anything while legacy ports configure.

  garage_constants();
  exit_condition_defaults();

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({{Auton("test", drivetest)}});

  // Initialize chassis and auton selector
  sylib::initialize();
  chassis.initialize();
  ez::as::initialize();

  catapult.startTask();
  lights.startTask();
}

void autonomous() {
  chassis.reset_pid_targets();               // Resets PID targets to 0
  chassis.reset_gyro();                      // Reset gyro position to 0
  chassis.reset_drive_sensor();              // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold. This helps
                                             // autonomous consistency.

  ez::as::auton_selector
      .call_selected_auton(); // Calls selected auton from autonomous selector.
}

void disabled() {
  // Store the time at the start of the loop
  std::uint32_t clock = sylib::millis();
  while (true) {
    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

void opcontrol() {
  ez::as::shutdown();
  Gif *gif = new Gif(cat_gif, lv_scr_act());
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);

  while (true) {
    chassis.arcade_standard(ez::SPLIT);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake = Intake::STATE::IN;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake = Intake::STATE::OUT;
    } else {
      intake = Intake::STATE::IDLE;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      if (catapult.getState() == balls::Catapult::State::Reloading) {
        catapult.fire();
      }
    } else {
      if (catapult.getState() == balls::Catapult::State::Firing) {
        catapult.stop();
      }
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      doinker.toggle();
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      blocker.toggle();
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      hang.toggle();

      if (!hang.getState()) {
        chassis.set_active_brake(0);
        chassis.set_drive_brake(MOTOR_BRAKE_COAST);
      } else {
        chassis.set_active_brake(0.1);
        chassis.set_drive_brake(MOTOR_BRAKE_HOLD);
      }
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}