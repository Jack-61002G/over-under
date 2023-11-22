#include "main.h"
#include "autoSelect/selection.h"
#include "autons.hpp"
#include "cata.hpp"
#include "gif-pros/gifclass.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "robotconfig.h"
#include "sylib/system.hpp"
#include <iostream>

ASSET(cat_gif)

void initialize() {

  // Initialize chassis and auton selector
  sylib::initialize();
  lights.startTask();
  lights.rotate();

  selector::init();

  chassis.calibrate();
  catapult.startTask();
}

void autonomous() {
  lights.loadFile();
  lights.setColor(selector::auton);

  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);

  if (std::abs(selector::auton) == 1) {
    closeSideMid();
  } else if (std::abs(selector::auton) == 2) {
    closeSide3();
  } else if (std::abs(selector::auton) == 3) {
    farSide();
  } else if (selector::auton == 0) {
    skills();
  }
}

void disabled() {
  lights.rotate();
  // Store the time at the start of the loop
  std::uint32_t clock = sylib::millis();
  while (true) {
    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

void opcontrol() {
  std::uint32_t clock = sylib::millis();
  lights.loadFile();
  lights.setColor(selector::auton);

  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

  Gif *gif = new Gif(cat_gif, lv_scr_act());

  bool cata_button = false;

  /*
    chassis.setPose(-40.5, 54.5, -90);
    // move to matchloader
    chassis.moveTo(-54, 48.2, -71, 1000);
    doinker.toggle();
    // shoot matchloads
    catapult.setState(balls::Catapult::State::Matchload);
    while (catapult.getState() == balls::Catapult::State::Matchload) {
      pros::delay(20);
    }
  */
  

  while (true) {
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                   controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 4);

    std::cout << cataMotor.get_efficiency() << std::endl;

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
        }
        cata_button = true;
      }
    } else {
      if (cata_button) {
        if (catapult.getState() == balls::Catapult::State::Firing) {
          catapult.setState(balls::Catapult::State::Idle);
        }
        cata_button = false;
      }
    }

    //if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
    //  catapult.setState(balls::Catapult::State::Matchload);
    //}

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      blocker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      Rwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      Lwingus.toggle();
    }

    sylib::delay_until(&clock, 10);
  }
}