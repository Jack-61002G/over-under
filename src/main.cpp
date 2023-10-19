#include "main.h"
#include "EZ-Template/auton.hpp"
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "gif-pros/gifclass.hpp"
#include "pros/misc.h"
#include "robotconfig.h"

ASSET(cat_gif)

void initialize() {
  sylib::initialize();

  // Print our branding over your terminal :D
  ez::print_ez_template();

  pros::delay(
      500); // Stop the user from doing anything while legacy ports configure.

  garage_constants();
  modified_exit_condition();

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    {Auton("test", drivetest)}
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();

  catapult.startTask();
  lights.startTask();

  intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
            doinkerLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
}

void autonomous() {

  chassis.reset_pid_targets();               // Resets PID targets to 0
  chassis.reset_gyro();                      // Reset gyro position to 0
  chassis.reset_drive_sensor();              // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps
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
  blocker.toggle();
   Gif *gif = new Gif(cat_gif, lv_scr_act());
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);

  while (true) {

    chassis.arcade_standard(ez::SPLIT);

    Intake::STATE meow =
        controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) // if
            ? Intake::STATE::OUT
            : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) // else if
                  ? Intake::STATE::IN
                  : Intake::STATE::IDLE; // else

    intake.setState(meow);

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      catapult.fire();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      doinker.toggle();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}