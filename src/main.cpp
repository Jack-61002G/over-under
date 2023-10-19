#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "lemlib/asset.hpp"
#include "pros/misc.h"
#include "robotconfig.h"

ASSET(cat_gif)
ASSET(skillsmatchload_txt)

void initialize() {
  sylib::initialize();
  Gif* gif = new Gif(cat_gif, lv_scr_act()); 
  catapult.startTask();
  lights.startTask();
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

  std::uint32_t clock = sylib::millis();
  catapult.changeTarget(55);

  while (true) {
    
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                   controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

    Intake::STATE meow = 
      controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)     //if
        ? Intake::STATE::IN
      : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) //else if
        ? Intake::STATE::OUT 
      : Intake::STATE::IDLE;                                           //else

    intake.setState(meow);

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      catapult.fire();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      doinker.toggle();
    }

    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}