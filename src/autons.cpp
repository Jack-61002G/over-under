#include "autons.hpp"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"

ASSET(skillspush_txt)
ASSET(disrupt_txt)
ASSET(farSideMidRushContact_txt)
ASSET(reallyjankpush_txt)

void driveFor(float distance, float timeout = 1500, bool direction = true, float speed = 127, float minspeed = 0) {
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
  chassis.moveToPose(-6, 59.5, 90, 1000);
  chassis.waitUntil(25);
  doinker.toggle();
  
  chassis.moveToPose(-55, 40, 0, 1800, {false, 13, .5, 70});
  doinker.toggle();
  pros::delay(150);
  chassis.moveToPose(-64, 0, 0, 600, {false});
  chassis.moveToPose(-60, 25, 0, 750);
  chassis.turnTo(-60, 0, 750);
  pros::delay(100);
  intake = Intake::STATE::OUT;
  chassis.moveToPose(-60, 15, 180, 650);
  intake = Intake::STATE::IDLE;
  chassis.moveToPose(-75, 30, 90, 1200, {false});
  chassis.setPose(-64, 35.5, 90);
  pros::delay(250);

  intake = Intake::STATE::IN;
  chassis.moveToPose(-20, 35.5, 90, 300);
  chassis.moveToPose(-11, 22, 90, 1000, {false, 0, 0.2});
  pros::delay(250);

  chassis.moveToPose(-13, 8, 0, 1000, {false, 0, 0});
  Rwingus.toggle();
  chassis.moveToPose(-65, 8, 90, 1000, {false});
  chassis.moveToPose(-30, 8, 90, 1000);
  chassis.turnTo(-60, 8, 1000);
  pros::delay(150);
  intake = Intake::STATE::OUT;
  chassis.moveToPose(-60, 8, -90, 1000);
  intake = Intake::STATE::IDLE;
  Rwingus.toggle();
  chassis.moveToPose(-30, 8, -90, 1000, {false});
}

void descore() {
  chassis.setPose(-45, -54.25, 0);
  Lwingus.toggle();
  pros::delay(150);

  // aim to slap the triball out
  intake = Intake::STATE::OUT;
  turnToAngle(-90, 250);
  turnToAngle(180, 1000);
  Lwingus.toggle();
  intake = Intake::STATE::IDLE;

  // score preload in goal
  chassis.moveToPose(-59, -21, 180, 1500, {false, 0, 0.75});

  // move that jawn to the elevation bar and knock those triballs to the other
  // side
  chassis.moveToPose(8.4, -62.5, 90, 10000, {true, 0, 0.85});
  chassis.waitUntil(10);
  chassis.waitUntilDone();
}

void disrupt() { // does not work

  chassis.setPose(-31, -54.25, 0);

  //51581s ball swap
  chassis.moveToPoint(-24, -7, 1000);
  chassis.waitUntil(15);
  Lwingus.set(true);
  intake = Intake::STATE::IN;
  chassis.waitUntil(25);
  Lwingus.set(false);
  chassis.waitUntilDone();

  chassis.follow(disrupt_txt, 17, 5400, false);
  intake = Intake::STATE::HOLD;
  chassis.waitUntil(60);
  doinker.set(true);
  chassis.waitUntil(85);
  doinker.set(false);
  chassis.waitUntilDone();
  turnToAngle(90, 500);
  chassis.moveToPoint(-7, -58, 1000);
}

void skills() {
  chassis.setPose(-41, 54.5, 180);

  pros::Task task{[=] {
    pros::delay(700);
    intake = Intake::STATE::OUT;
  }};

  // push preload into goal
  chassis.moveToPose(-60, -5, 180, 1200);

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-57.5, 43.5, 108, 1500, {false});
  Lwingus.toggle();

  catapult.setState(balls::Catapult::State::Matchload);

  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }

  Lwingus.toggle();
  pros::delay(250);

  chassis.moveToPose(-50, 40, 240, 1100);

  pros::Task task2electricboogaloo{[=] {
    pros::delay(2600);
    Rwingus.toggle();
    pros::delay(650);
    Lwingus.toggle();
    pros::delay(700);
    Lwingus.toggle();
  }};

  // first push
  chassis.follow(skillspush_txt, 17, 5400, false, false);

  Rwingus.toggle();
  pros::delay(100);

  // back out
  chassis.moveToPose(chassis.getPose().x - 20, chassis.getPose().y, -90, 1200);
  turnToAngle(0, 500);
  pros::delay(100);

}