#include "main.h"
#include "robotconfig.h"

void screen() {
  // loop forever
  while (true) {
    lemlib::Pose pose =
        chassis.getPose(); // get the current position of the robot
    pros::lcd::print(0, "x: %f", pose.x);           // print the x position
    pros::lcd::print(1, "y: %f", pose.y);           // print the y position
    pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
    pros::delay(10);
  }
}

void initialize() {

  pros::lcd::initialize();
  chassis.calibrate();
  pros::Task screenTask(
      screen); // create a task to print the position to the screen
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  chassis.VelController('l', 2.2, 1.7, 23, 7.5);
  chassis.VelController('r', 2.2, 1.7, 23, 7.5);
  trajectory nya(Pose(0, 0, 0), Pose(11.59, 50, 1.2), 10, 27, 5, 5, 5.1, 1);
  chassis.followProfile(nya);
}

void opcontrol() {
  pros::Motor intake_motor(4, false);
  std::shared_ptr<Intake> intake = std::make_shared<Intake>(intake_motor);

  while (true) {

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake->setState(Intake::STATE::IN);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake->setState(Intake::STATE::OUT);
    } else {
      intake->setState(Intake::STATE::IDLE);
    }

    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                   controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
  }
}
