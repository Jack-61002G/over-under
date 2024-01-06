#include "autons.hpp"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"

ASSET(disruptSwap_txt)
ASSET(disruptClear_txt)

ASSET(skillspush_txt)
ASSET(reallyjankpush_txt)

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
  intake = Intake::STATE::IN;
  chassis.moveToPose(-8, 59.5, 90, 1000);

  chassis.moveToPose(-60, 40, 0, 1800, {false, 13, .35, 127});
  chassis.moveToPose(-60, 0, 0, 650, {false, 0, .6, 127, 127});
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, 30.75, chassis.getPose().theta);
  pros::delay(100);

  chassis.moveToPose(-60, 36, 0, 750);
  chassis.turnTo(-60, 0, 750);
  chassis.waitUntilDone();
  pros::delay(100);
  intake = Intake::STATE::OUT;
  chassis.moveToPose(-60, 24, 180, 650, {true, 0, .6, 127, 127});
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

  chassis.moveToPose(0, -60, 90, 1500, {true, 0, 0.5, 80});
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

void skills() { // borked
  chassis.setPose(-47, 57.5, 225);

  // push preload into goal
  chassis.moveToPose(-57.5, 5, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-54.5, 45, -71, 1500, {false, 0, 0.2});
  chassis.waitUntilDone();

  Rwingus.toggle();

  //catapult.setState(balls::Catapult::State::Matchload);
  chassis.setPose(-55, 47, chassis.getPose().theta);
  pros::delay(1000);
  
  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }

  Rwingus.toggle();

  chassis.turnTo(-29, 60, 1000);
  chassis.waitUntilDone();
  chassis.follow(skillspush_txt, 10, 5400);
  chassis.waitUntilDone();
  intake = Intake::STATE::OUT;

  chassis.moveToPoint(chassis.getPose().x+40, chassis.getPose().y, 1000);
  chassis.waitUntilDone();
  
  // back out
  chassis.moveToPoint(12, 12, 1000, false);

  // scooch over
  chassis.turnTo(10, 0, 1000);
  chassis.moveToPoint(10, 0, 1000);
  chassis.turnTo(60, 0, 1000);
  chassis.waitUntilDone();

  Lwingus.set(true);
  Rwingus.set(true);

  // second push
  chassis.moveToPose(60, 0, 90, 1000);
  chassis.waitUntilDone();

  Lwingus.set(false);
  Rwingus.set(false);
  pros::delay(100);

  // back out
  chassis.moveToPoint(12, 0, 1000, false);

  // scooch over
  chassis.turnTo(10, -30, 1000);
  chassis.moveToPoint(10, -30, 1000);
  chassis.turnTo(60, 0, 1000);

  // third push
  chassis.moveToPose(60, 0, 90, 2000);
  TOGGLE_WINGS;

  // back out
  chassis.moveToPoint(10, -12, 1000, false);
}