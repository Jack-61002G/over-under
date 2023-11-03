#include "main.h"
#include "cata.hpp"
#include "gif-pros/gifclass.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "robotconfig.h"
#include "sylib/system.hpp"
#include "autoSelect/selection.h"

ASSET(cat_gif)
ASSET(path_txt)

void initialize() {

  // Initialize chassis and auton selector
  selector::init();
  sylib::initialize();
  chassis.calibrate();

  catapult.startTask();
  lights.startTask();
}

void autonomous() {
  if(selector::auton == 1){
    chassis.setPose(-17, 59.5, 90);
    intake = Intake::STATE::IN;
    chassis.moveTo(-6, 59.5, 90, 1000);
    chassis.moveTo(-64, 20, 0, 2350, false, false, 13, .6);
    chassis.setPose(-64, 29.625, 0);
    chassis.moveTo(-60, 34, 0, 5000);
    chassis.turnTo(chassis.getPose().x, -60, 5000);
    intake = Intake::STATE::OUT;
    chassis.moveTo(chassis.getPose().x, 10, 180, 750);
    intake = Intake::STATE::IDLE; 
  }
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
  std::uint32_t clock = sylib::millis();
  
  Gif *gif = new Gif(cat_gif, lv_scr_act());

  bool cata_button = false;

  while (true) {
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 4);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake = Intake::STATE::IN;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake = Intake::STATE::OUT;
    } else {
      intake = Intake::STATE::IDLE;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      if (!cata_button) {
        if (catapult.getState() == balls::Catapult::State::Matchload) {
          catapult.setState(balls::Catapult::State::Idle);
        } else {
          catapult.setState(balls::Catapult::State::Firing);
        } cata_button = true;
      }
    } else {
      if (cata_button) {
        if (catapult.getState() == balls::Catapult::State::Firing) {
          catapult.setState(balls::Catapult::State::Idle);
        } cata_button = false;
      }
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      catapult.setState(balls::Catapult::State::Matchload);
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      doinker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      blocker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      hang.toggle();
    }

    sylib::delay_until(&clock, 10);
  }
}