#include "main.h"
#include "EZ-Template/util.hpp"
#include "autoSelect/selection.h"
#include "autons.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"



void initialize() {

  // initializing auton selector
  
  pros::delay(500);
  sylib::initialize();

  // initializing objects
  chassis.initialize();
  
  //lemChassis.calibrate(false);


  // setting up the drive constants
  garage_constants();
  exit_condition_defaults();
  selector::init();
  lights.startTask();
}



void autonomous() {

  chassis.reset_pid_targets();               // Resets PID targets to 0
  chassis.reset_gyro();                      // Reset gyro position to 0
  chassis.reset_drive_sensor();              // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
  

  // running the selected autonomous route
  if (std::abs(selector::auton) == 1) {
    WPclose();
  } else if (std::abs(selector::auton) == 2) {
    WPfar();
  } else if (std::abs(selector::auton) == 3) {
    SAFEclose();
  } else if (std::abs(selector::auton) == 4) {
    RUSHclose();
  } else if (selector::auton == 5) {
    RUSHfar();
  }
}



void disabled() {

  // making sure the wings are retracted
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

  // Storing the current time
  std::uint32_t clock = sylib::millis();

  chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
  chassis.set_mode(ez::DISABLE);
  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);


  // Driver control loop
  while (true) {

    // chassis control
    int turnVal = lemlib::defaultDriveCurve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 4);
    int latVal = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

    if (std::abs(latVal) > 114 && std::abs(turnVal) > 114) {
      if (latVal < 0) {
        latVal = -110;
      } else {
        latVal = 110;
    }}

    lemChassis.arcade(latVal, turnVal);


    // intake control
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake = Intake::STATE::IN;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake = Intake::STATE::OUT;
    } else {
      intake = Intake::STATE::IDLE;
    }


    // wing control
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      Rwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      Lwingus.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      hook.set(true);
    }


    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}