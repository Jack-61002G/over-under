#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "lemlib/asset.hpp"
#include "pros/misc.h"
#include "robotconfig.h"

ASSET(h_gif)
ASSET(skillsmatchload_txt)

void initialize() {
  sylib::initialize();
}

void autonomous() { chassis.follow(skillsmatchload_txt, 5000, 5); }

void disabled() {

  // Store the time at the start of the loop
  std::uint32_t clock = sylib::millis();
  while (true) {

    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

void opcontrol() {
  // catapult.startTask();

  std::uint32_t clock = sylib::millis();

  while (true) {
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                   controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

    // intake controller input
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
      if (intake.getState() != Intake::STATE::IN) {
        intake.setState(Intake::STATE::IN);
      } else {
        intake.setState(Intake::STATE::IDLE);
      }
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
      if (intake.getState() != Intake::STATE::OUT) {
        intake.setState(Intake::STATE::OUT);
      } else {
        intake.setState(Intake::STATE::IDLE);
      }
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      catapult.fire();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      LeftWingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      RightWingus.toggle();
    }

    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}