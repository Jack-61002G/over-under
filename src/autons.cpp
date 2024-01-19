#include "autons.hpp"
#include "cata.hpp"
#include "intake.h"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"

ASSET(disruptSwap_txt)

ASSET(skillspush_txt)
ASSET(reallyjankpush_txt)

void holdAngle(double angle) {
  pros::Task task{[=] {

            const int kP = 7;
            const double kD = 12.0;
            const double drift = 10.0;

            double prevError = 0;

            while (catapult.getState() == balls::Catapult::State::Matchload) {
              double error = angle - chassis.getPose().theta;

              double derivitive = error - prevError;
              prevError = error;


              double output = error * kP + derivitive * kD;

              leftMotors = output + drift;
              rightMotors = -output + drift;

              pros::delay(20);
  

            
            }
    }};
}


void holdPose(double x, double y, double theta) {
  pros::Task task{[=] {
            
            while(catapult.getState() == balls::Catapult::State::Matchload) {
              chassis.moveToPose(x, y, theta, 500);
              chassis.waitUntilDone();
              pros::delay(50);
            }

  }};}


void driveFor(float distance, float timeout = 1500, bool direction = true,
              float speed = 127, float minspeed = 0) {
  chassis.moveToPose(
      chassis.getPose().x + distance * std::sin(chassis.getPose().theta),
      chassis.getPose().y + distance * std::cos(chassis.getPose().theta),
      chassis.getPose().theta, timeout, {direction, 0, 0, speed, minspeed});
  chassis.waitUntilDone();
}
void turnToAngle(double desiredTheta, double timeout) {
  double radians = desiredTheta * M_PI / 180.0;

  double newX = chassis.getPose().x + 30.0 * cos(radians);
  double newY = chassis.getPose().y + 30.0 * sin(radians);

  chassis.turnTo(newX, newY, timeout);
}

#define TOGGLE_WINGS                                                           \
  Lwingus.toggle();                                                            \
  Rwingus.toggle();

// autos

void sixBall() {

  chassis.setPose(-17, 59.5, 90);

  // grab ball under the elevation bar
  intake = Intake::STATE::IN;
  chassis.moveToPose(-8, 59.5, 90, 1000);

  // back up into the goal
  chassis.moveToPose(-66, 40, 0, 1800, {false, 13, .35, 90});
  chassis.moveToPose(-66, 0, 0, 1100, {false, 0, .6, 127, 127});
  chassis.waitUntilDone();
  
  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
  pros::delay(800);

  // reset pos
  chassis.setPose(chassis.getPose().x, 30.75, chassis.getPose().theta);
  leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
  pros::delay(100);

  // out of the goal
  chassis.moveToPose(-66, 36, 0, 750);
  chassis.turnTo(-66, 0, 750);
  chassis.waitUntilDone();

  // forward push into the goal
  pros::delay(100);
  intake = Intake::STATE::OUT;
  chassis.moveToPose(-66, 0, 180, 1300, {true, 0, .6, 127, 127});
  intake = Intake::STATE::IDLE;

  chassis.moveToPoint(-40, 48, 500, false);
  chassis.turnTo(-24, 24, 500);


  // mid balls
  intake = Intake::STATE::IN;
  chassis.moveToPoint(-13, 30, 1000);

  chassis.turnTo(-20, 22, 1000);
  chassis.waitUntilDone();
  chassis.moveToPoint(-15, 23, 1000);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();
  pros::delay(350);

  chassis.turnTo(0, 0, 1000);
  intake = Intake::STATE::IN;
  chassis.moveToPose(0, 8, 115, 1000, {true, 100000, 0.3});
  chassis.turnTo(-45, 3, 1000);
  chassis.waitUntilDone();
  intake = Intake::STATE::HOLD;
  TOGGLE_WINGS;
  chassis.moveToPoint(-45.491, 3, 1000);
  chassis.waitUntil(20);
  intake = Intake::STATE::OUT;
  chassis.moveToPoint(-24, 0, 1000, false);
  intake = Intake::STATE::IDLE;
  TOGGLE_WINGS;
}

void farSideSafe() {
  chassis.tank(-127, -127);
  pros::delay(1000);
  chassis.tank(0, 0);
  pros::delay(100);
  chassis.tank(80, 80);
  pros::delay(200);
  chassis.tank(0, 0);
}

void descore() {
  chassis.setPose(-46.25, -54.5, 0);
  Lwingus.set(true);
  pros::delay(150);

  // slap the triball out
  chassis.turnTo(-100, -55, 600);
  chassis.waitUntilDone();
  pros::delay(400);

  // turn back
  turnToAngle(0, 50);
  Lwingus.set(false);
  turnToAngle(100, 400);

  chassis.moveToPose(0.5, -62, 90, 1500, {true, 0, 0.5, 80});
  chassis.waitUntilDone();

  intake = Intake::STATE::OUT;
  pros::delay(1000);
  intake = Intake::STATE::IDLE;
}

void disrupt() { // does not work

  chassis.setPose(-34, -54, 0);

  chassis.moveToPoint(-34, -12.5, 1000, true, 100); // rush in
  chassis.waitUntilDone();

  chassis.moveToPose(-34, -12.5, 90, 500); // turn
  Lwingus.set(true);
  chassis.waitUntilDone();

  intake = Intake::STATE::OUT;
  chassis.moveToPoint(0, -12.5, 1000, true, 100); // descore over barrier
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;
  chassis.moveToPoint(-20, -12.5, 1000, false); // back into the center

}

void swap() {

  chassis.setPose(-34, -54, 0);
  intake = Intake::STATE::IN;
  chassis.follow(disruptSwap_txt, 15, 1500);
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x, -11.5, 1000);
  chassis.turnTo(0, -11.5, 500);
  chassis.waitUntilDone();
  Lwingus.set(true);
  intake = Intake::STATE::OUT;
  chassis.moveToPoint(0, -11.5, 1000, true);
  chassis.waitUntilDone();
  Lwingus.set(false);
  chassis.moveToPoint(-20, -11.5, 1000, false);
  pros::delay(200);
  intake = Intake::STATE::IDLE;

  chassis.turnTo(-20, -30, 1000);
  chassis.moveToPoint(-20, -30, 1000);

}

void skills() {

  chassis.setPose(-47, 57.5, 225);

  // push preload into goal
  chassis.moveToPose(-57.5, 5, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-54.5, 45, -66.5, 1500, {false, 0, 0.2});
  chassis.waitUntilDone();

  Rwingus.toggle();
  pros::delay(100);

  catapult.setState(balls::Catapult::State::Matchload);
  chassis.setPose(-55, 47, chassis.getPose().theta);
  holdAngle(294);
  pros::delay(1000);
  
  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }

  Rwingus.toggle();

  chassis.turnTo(-29, 55, 1000, false);
  chassis.waitUntilDone();
  chassis.follow(skillspush_txt, 14, 6000, false);
  chassis.waitUntilDone();

  chassis.turnTo(60, 1, 1000);
  chassis.waitUntilDone();
  chassis.moveToPoint(60, 6, 1000);
  chassis.waitUntilDone();

  chassis.turnTo(100, chassis.getPose().y, 1000);
  chassis.waitUntilDone();
  
  // back out
  chassis.moveToPoint(12, 12, 1000, false);

  // scooch over
  chassis.turnTo(12, -12, 1000);
  chassis.moveToPoint(12, 0, 1000);
  chassis.turnTo(60, 0, 1000);
  chassis.waitUntilDone();

  Lwingus.set(true);
  Rwingus.set(true);

  // second push
  chassis.moveToPose(65, 0, 90, 1200, {true, 0, 0.2, 127, 100});
  chassis.waitUntilDone();

  chassis.tank(-100, -100);
  pros::delay(500);
  chassis.tank(0, 0);
  pros::delay(300);

  chassis.moveToPose(65, 0, 90, 1000, {true, 0, 0.2, 127, 100});
  chassis.waitUntilDone();

  Lwingus.set(false);
  Rwingus.set(false);
  pros::delay(100);

  // back out
  chassis.moveToPoint(11, 0, 1000, false);

  // scooch over
  chassis.turnTo(12, -30, 1000);
  chassis.moveToPoint(12, -30, 1000);
  chassis.turnTo(60, 0, 1000);

  // third push
  chassis.moveToPose(60, -1, 90, 2000);
  TOGGLE_WINGS;
  chassis.waitUntilDone();
  TOGGLE_WINGS;

  // back out
  chassis.moveToPoint(10, chassis.getPose().y, 1000, false);

  //barrier cross
  chassis.turnTo(-2, -70, 1500);
  chassis.waitUntilDone();
  chassis.tank(127, 127);
  pros::delay(2500);
  chassis.tank(0, 0);

  //wall reset
  chassis.setPose(15, -64, 180);
  chassis.tank(-100, -100);
  pros::delay(150);
  chassis.tank(0, 0);

  //turn to path 
  chassis.turnTo(100, -60, 1000);
  chassis.waitUntilDone();

  //follow path
  chassis.follow(reallyjankpush_txt, 13, 2500);
  chassis.waitUntilDone();

  //get out
  chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y-18, 1200, false);
  chassis.waitUntilDone();

  chassis.moveToPoint(63.5, chassis.getPose().y+50, 1200);
  chassis.waitUntilDone();

  chassis.moveToPoint(63.5, chassis.getPose().y-18, 1200, false);


}