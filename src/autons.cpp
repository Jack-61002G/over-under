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
  chassis.moveToPose(-60, 0, 0, 600, {false, 0, .6, 127, 127});
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

  //mid balls
  intake = Intake::STATE::IN;
  chassis.moveToPoint(-13, 30, 1000);


  chassis.turnTo(-20, 24, 1000);
  chassis.waitUntilDone();
  chassis.moveToPoint(-15, 24, 1000);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();
  pros::delay(350);

  chassis.turnTo(0, 0, 1000);
  intake = Intake::STATE::IN;
  chassis.moveToPose(0, 8, 115,1000, {true, 100000, 0.3});
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
  chassis.setPose(-47.25, -55.5, 0);
  doinker.toggle();
  pros::delay(150);

  // aim to slap the triball out
  chassis.turnTo(chassis.getPose().x-30, -55.5, 500);
  chassis.moveToPoint(-36, -60, 800, false);
  doinker.toggle();

  chassis.turnTo(chassis.getPose().x+30, -60, 1000);
  chassis.moveToPoint(-14.5, -60, 3000, true, 70);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();
  pros::delay(500);
  intake = Intake::STATE::IDLE;

}

void disrupt() { // does not work

  chassis.setPose(-31, -55.25, 0);

  // 51581s
  chassis.follow(disruptSwap_txt, 10, 2000);
  chassis.waitUntil(30);
  intake = Intake::STATE::IN;
  chassis.waitUntilDone();

  // clear and get ready to bowl
  chassis.follow(disruptClear_txt, 10, 5400, false);
  intake = Intake::STATE::HOLD;
  chassis.waitUntil(10);
  chassis.waitUntil(58);
  doinker.set(true);
  chassis.waitUntil(65);
  doinker.set(false);
  chassis.turnTo(30, -58, 1000);
  chassis.moveToPoint(-10, -58, 1000);
}

void skills() {
  chassis.setPose(-41, 54.5, 180);

  // push preload into goal
  chassis.moveToPose(-60, -5, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-57.5, 43.5, -78, 1500);
  Rwingus.toggle();

  catapult.setState(balls::Catapult::State::Matchload);

  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }

  Rwingus.toggle();

  chassis.moveToPose(-50, 40, 240, 1100);

  // first push
  chassis.follow(skillspush_txt, 17, 5400);
  chassis.waitUntil(85);
  Lwingus.set(true);
  chassis.waitUntil(135);
  Rwingus.set(true);
  chassis.waitUntilDone();
  Lwingus.set(false);
  Rwingus.set(false);

  // back out
  driveFor(-24);
  turnToAngle(180, 500);
  pros::delay(100);
}