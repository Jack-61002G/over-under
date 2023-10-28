#include "autons.hpp"
#include "EZ-Template/util.hpp"
#include "robotconfig.h"
#include "main.h"
#include "pros/adi.h"
#include "pros/rtos.hpp"

// #include "intake.cpp"

///
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
///

const int DRIVE_SPEED =
    110; // This is 110/127 (around 87% of max speed).  We don't suggest making
        // this 127. If this is 127 and the robot tries to heading correct, it's
        // only correcting by making one side slower.  When this is 87%, it's
        // correcting by making one side faster and one side slower, giving
        // better heading correction.
const int TURN_SPEED = 110;
const int SWING_SPEED = 100;

// It's best practice to tune constants when the robot is empty and with heavier
// game objects, or with lifts up vs down. If the objects are light or the cog
// doesn't change much, then there isn't a concern here.

void garage_constants() {
  chassis.set_slew_min_power(70, 70);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 12, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, .45, 0, 4, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, .45, 0, 4, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 47, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 50, 0);
}

void exit_condition_defaults() {
  chassis.set_exit_condition(chassis.turn_exit, 35, 1, 150, 3, 250, 500);
  chassis.set_exit_condition(chassis.swing_exit, 50, 2, 250, 5, 250, 500);
  chassis.set_exit_condition(chassis.drive_exit, 35, 30, 150, 150, 250, 500);
}

void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void closeSide3Ball() {

  intake = Intake::STATE::IN;
  chassis.set_drive_pid(12, 50, true);
  chassis.wait_drive();
  pros::delay(250);
  chassis.set_drive_pid(-38, 110, true);
  chassis.wait_until(35);
  chassis.set_turn_pid(-45, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(-30, 110);
  chassis.wait_until(-28);
  chassis.set_turn_pid(-90, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(-14, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(10, 110);
  chassis.wait_drive();
  chassis.set_turn_pid(90, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(12, 110);
  intake = Intake::STATE::OUT;
  chassis.wait_drive();
  chassis.set_drive_pid(-12, 110);
  chassis.wait_drive();
  intake = Intake::STATE::IDLE;
  chassis.set_turn_pid(13, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(50, 110);
  chassis.wait_until(25);
  intake = Intake::STATE::IN;
  chassis.wait_drive();
  pros::delay(250);
  chassis.set_drive_pid(-50, 110);
  chassis.wait_drive();
  chassis.set_turn_pid(90, 110);
  chassis.wait_drive();
  intake = Intake::STATE::OUT;
  chassis.set_drive_pid(12, 110);
  chassis.wait_drive();
  chassis.set_drive_pid(-12, 110);
  chassis.wait_drive();
  intake = Intake::STATE::IDLE;
  
}