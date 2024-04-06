#include "autons.hpp"
#include "EZ-Template/util.hpp"
#include "intake.h"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"



const int DRIVE_SPEED = 110;
const int TURN_SPEED = 102;
const int SWING_SPEED = 97;



void garage_constants() {
  chassis.set_slew_min_power(70, 70);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 12, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, .55, 0, 5.5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, .55, 0, 5.5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 4, 0.003, 40, 15);

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



void WPclose() { // descore

  chassis.set_angle(190);

  chassis.set_drive_pid(48.3, 127); // grab middle ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  chassis.set_turn_pid(209, TURN_SPEED); // turn to matchloader
  chassis.wait_drive();

  pros::delay(150);
  intake = Intake::STATE::IDLE;

  chassis.set_drive_pid(-52, DRIVE_SPEED); // back up
  chassis.wait_drive();

  Lwingus.set(true);

  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_swing_pid(ez::LEFT_SWING, 93, SWING_SPEED); // descore
  chassis.wait_drive();
  chassis.set_drive_pid(-9, DRIVE_SPEED);
  chassis.wait_drive();

  Lwingus.set(false);

  chassis.set_turn_pid(-90, TURN_SPEED); // turn to alley
  chassis.wait_drive();

  chassis.set_drive_pid(30, DRIVE_SPEED); // touch the bar
  intake = Intake::STATE::OUT;
  chassis.wait_drive();
  pros::delay(1000);
  intake = Intake::STATE::IDLE;
}



void WPfar() { // five ball safe

  chassis.set_angle(45);

  Lwingus.set(true);
  Rwingus.set(true);

  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();

  Rwingus.set(false);

  chassis.set_swing_pid(ez::LEFT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-26, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(12, DRIVE_SPEED); // back up
  Lwingus.set(false);
  chassis.wait_drive();

  chassis.set_turn_pid(110, TURN_SPEED); // turn to first ball
  chassis.wait_drive();

  chassis.set_drive_pid(48, DRIVE_SPEED); // grab first ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  pros::delay(500);
  intake = Intake::STATE::IDLE;

  chassis.set_turn_pid(242, TURN_SPEED); // turn to goal
  chassis.wait_drive();

  pros::delay(200);
  intake = Intake::STATE::SPIT; // outtake to goal
  pros::delay(400);
  intake = Intake::STATE::IDLE;

  chassis.set_turn_pid(170, TURN_SPEED); // turn to mid ball
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED); // grab mid ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  pros::delay(500);
  intake = Intake::STATE::IDLE;

  chassis.set_turn_pid(90, TURN_SPEED); // turn away from goal
  chassis.wait_drive();

  Lwingus.set(true);
  Rwingus.set(true);

  chassis.set_drive_pid(-38, 127); // push balls into goal
  chassis.wait_drive();
  chassis.set_drive_pid(12, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED);

  Lwingus.set(false);
  Rwingus.set(false);
  chassis.wait_drive();

  intake = Intake::STATE::OUT;
  pros::delay(150);
  chassis.set_drive_pid(20, 127);
  chassis.wait_drive();
  intake = Intake::STATE::IDLE;
  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();
}



void SAFEclose() { // non-rush WP descore

  chassis.set_drive_pid(4, 100);
  chassis.wait_drive();

  Lwingus.set(true);

  chassis.set_swing_pid(ez::LEFT_SWING, -45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-15, DRIVE_SPEED);
  pros::delay(200);
  Lwingus.set(false);
  chassis.wait_drive();

  chassis.set_turn_pid(137, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(28, DRIVE_SPEED);
  intake = Intake::STATE::OUT;
  chassis.wait_drive();

  pros::delay(1000);
  intake = Intake::STATE::IDLE;

}



void RUSHclose() { // disrupt

  WPclose();

  chassis.set_drive_pid(-56, DRIVE_SPEED); // back up to matchload
}



void RUSHfar() { // six ball rush

  chassis.set_angle(170);

  Rwingus.set(true);

  chassis.set_drive_pid(48.3, 127); // grab middle ball
  pros::delay(300);

  intake = Intake::STATE::IN;
  Rwingus.set(false);

  chassis.wait_drive();
  chassis.set_turn_pid(160, TURN_SPEED); // turn to matchloader

  pros::delay(50);
  intake = Intake::STATE::IDLE;
  pros::delay(150);

  chassis.set_drive_pid(-54, DRIVE_SPEED); // back up
  chassis.wait_drive();


  chassis.set_turn_pid(220, TURN_SPEED); // turn to outtake to goal
  
  pros::delay(250);
  intake = Intake::STATE::OUT;
  pros::delay(300);
  
  chassis.set_turn_pid(87, 120); // turn to elevation bar
  pros::delay(100);
  intake = Intake::STATE::IDLE;
  chassis.wait_drive();

  chassis.set_drive_pid(36, 127); // grab ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  pros::delay(100);

  chassis.set_drive_pid(-52, 127); // come back to matchloader

  chassis.wait_until(-16);
  intake = Intake::STATE::IDLE;

  chassis.wait_until(-28);
  Lwingus.set(true);
  pros::delay(50);

  chassis.set_swing_pid(ez::LEFT_SWING, 7, 80); // descore
  chassis.wait_drive();


  chassis.set_drive_pid(-30, 127); // push into goal
  pros::delay(800);

  chassis.set_drive_pid(10, 127); // drive out of goal
  Lwingus.set(false);
  pros::delay(150);
  chassis.set_swing_pid(ez::RIGHT_SWING, -90, SWING_SPEED);
  pros::delay(300);
  chassis.set_turn_pid(-180, TURN_SPEED);
  chassis.wait_drive();

  intake = Intake::STATE::OUT;
  pros::delay(50);

  chassis.set_drive_pid(16, 127); // push with front

  pros::delay(650);
  intake = Intake::STATE::IDLE;

  chassis.set_drive_pid(-12, 127); // back out of goal
  pros::delay(100);

  chassis.set_swing_pid(LEFT_SWING, -250, TURN_SPEED); // turn to first ball
  chassis.wait_drive();

  chassis.set_drive_pid(49.5, DRIVE_SPEED); // grab first ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  chassis.set_turn_pid(-117, TURN_SPEED); // turn to goal
  pros::delay(100);
  intake = Intake::STATE::IDLE;
  chassis.wait_drive();

  intake = Intake::STATE::SPIT; // outtake to goal
  pros::delay(400);

  chassis.set_turn_pid(-190, 110); // turn to mid ball
  pros::delay(100);
  intake = Intake::STATE::IDLE;
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED); // grab mid ball
  intake = Intake::STATE::IN;
  chassis.wait_drive();

  chassis.set_turn_pid(-90, 110); // turn to goal
  chassis.wait_drive();

  chassis.set_drive_pid(48, 127);
  pros::delay(50);
  intake = Intake::STATE::OUT;
  chassis.wait_until(39.5);
  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();
}