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
  lights.rotate();

  selector::init();

  chassis.calibrate();
  catapult.startTask();
  lights.startTask();
  pros::Task intakeButtonTask(intakeButtonTask_func);

  pros::delay(1000);

  // logo.set_all(0xFF0000);
}

void autonomous() {
  lights.auton = selector::auton;

  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);

  // Release intake in a lambda task
    pros::Task intakeReleaseTask([] {
    catapult.setState(balls::Catapult::State::Firing);
    pros::delay(750);
    catapult.setState(balls::Catapult::State::Idle);
  });
  
  rush();
  return;

  if (std::abs(selector::auton) == 1) {
    rush();
  } else if (std::abs(selector::auton) == 2) {
    disrupt();
  } else if (std::abs(selector::auton) == 3) {
    farSideSafe();
  } else if (std::abs(selector::auton) == 4) {
    descore();
  } else if (selector::auton == 0) {
    skillsNew();
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

  bool ptoState = false;

  std::uint32_t clock = sylib::millis();

  lights.auton = selector::auton;

  bool cata_button = false;



  // Matchloading macro
/*
  if (selector::auton == 0) {

  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);

  chassis.setPose(-47, 57.5, 225);

  // push preload into goal
  chassis.moveToPose(-58, -10, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-57.5, 37, -70, 1500, {false, 0, 0.2});
  chassis.waitUntilDone();

  Rwingus.toggle();
  pros::delay(100);

  catapult.setState(balls::Catapult::State::Firing);

  chassis.setPose(-55, 47, chassis.getPose().theta);
  holdAngle(290);
  pros::delay(20100);

  leftDriveLED.set_all(0x990000);
  rightDriveLED.set_all(0x990000);
  frontLED.set_all(0x990000);
  backLED.set_all(0x990000);
  pros::delay(1500);

  lights.setColor(leftDriveLED);
  lights.setColor(rightDriveLED);
  lights.setColor(frontLED);
  lights.setColor(backLED);
  catapult.setState(balls::Catapult::State::Idle);

  Rwingus.toggle();
  }
*/
  // end macro



  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

  while (true) {

    // chassis control
    int turnVal = lemlib::defaultDriveCurve(
        controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 4);
    int latVal = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    if (std::abs(latVal) > 114 && std::abs(turnVal) > 114) {
      if (latVal < 0) {
        latVal = -102;
      } else {
        latVal = 102;
      }
    }
    chassis.arcade(latVal, turnVal);

    // std::cout << cataMotor.get_efficiency() << std::endl;

    // intake control
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake = Intake::STATE::IN;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake = Intake::STATE::OUT;
    } else {
      intake = Intake::STATE::IDLE;
    }

    // catapult control
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      if (!cata_button) {
        catapult.setState(balls::Catapult::State::Firing);
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

    // pistons
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      doinker.toggle();
      if (doinker.getState() == true) {
        leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
        rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
      } else {
        leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
        rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
      }
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      Rwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      Lwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      pto.set_value(!ptoState);
      ptoState = !ptoState;
    }

    sylib::delay_until(&clock, 10);
  }
}