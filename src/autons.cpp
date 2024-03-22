#include "autons.hpp"
#include "intake.h"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "robotconfig.h"

const int DRIVE_SPEED = 110;
const int TURN_SPEED = 110;
const int SWING_SPEED = 100;

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

void WPclose() {

    chassis.set_drive_pid(50, 127);
    intake = Intake::STATE::IN;
    chassis.wait_drive();
    // come back to matchloader

    // extend wing and kick triball out of matchload zone

    // outtake ball and push down alley
}

void WPfar() {

    // back into goal

    // swing around goal and intake first ball

    // outtake towards goal

    // intake second ball

    // wing push into goal

    // turn around and push in second ball
}

void RUSHclose() {

  WPclose();

  // drive back and set up for bowling
}

void RUSHfar() {}