#include "lemlib/asset.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <iostream>

ASSET(skillspush_txt)
ASSET(farSideMidRush_txt)
ASSET(farSideMidRushContact_txt)

#define RAM(left_speed, right_speed, duration)                                 \
  do {                                                                         \
    chassis.tank(left_speed, right_speed);                                     \
    pros::delay(duration);                                                     \
    chassis.tank(0, 0);                                                        \
  } while (0)

#define WAIT_UNTIL_DONE chassis.waitUntilDist(-1);

#define TOGGLE_WINGS                                                           \
  Lwingus.toggle();                                                            \
  Rwingus.toggle();

#define TurnToAngle(desiredTheta, timeout)                                     \
  chassis.moveTo(chassis.getPose().x, chassis.getPose().y, desiredTheta,       \
                 timeout);

void closeSide3() {
  chassis.setPose(-17, 59.5, 90);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 59.5, 90, 1000);

  // clear matchloader and back up
  pros::Task task{[=] {
    pros::delay(1200);
    Lwingus.toggle();
  }};
  chassis.moveTo(-55, 40, 0, 1800, false, false, 13, .5, 70);
  Lwingus.toggle();
  pros::delay(500);
  chassis.moveTo(-64, 0, 0, 600, false, false);
  chassis.moveTo(-60, 25, 0, 750);
  chassis.turnTo(-60, 0, 750);
  pros::delay(100);
  intake = Intake::STATE::OUT;
  chassis.moveTo(-60, 15, 180, 650);
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-75, 30, 90, 1200, false, false);
  chassis.setPose(-64, 35.5, 90);
  pros::delay(250);

  intake = Intake::STATE::IN;
  chassis.moveTo(-20, 35.5, 90, 300);
  chassis.moveTo(-11, 22, 90, 1000, false, true, 0, 0.2);
  pros::delay(250);

  chassis.moveTo(-13, 8, 0, 1000, false, false, 0, 0);
  Rwingus.toggle();
  chassis.moveTo(-65, 8, 90, 1000, false, false);
  chassis.moveTo(-30, 8, 90, 1000);
  chassis.turnTo(-60, 8, 1000);
  pros::delay(150);
  intake = Intake::STATE::OUT;
  chassis.moveTo(-60, 8, -90, 1000);
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-30, 8, -90, 1000, false, false);
}

void closeSideMid() { // this shawty does NOT work
  closeSide3();
}

void farSide() {
  chassis.setPose(-45, -54.25, 0);
  Lwingus.toggle();
  pros::delay(150);

  // aim to slap the triball out
  intake = Intake::STATE::OUT;
  TurnToAngle(-90, 250);
  TurnToAngle(180, 1000);
  Lwingus.toggle();
  intake = Intake::STATE::IDLE;

  // score preload in goal
  chassis.moveTo(-59, -23, 180, 1500, false, false, 0, 0.75);

  // move that jawn to the elevation bar and knock those triballs to the other
  // side
  chassis.moveTo(13, -63, 90, 10000, true, true, 0, 0.85);
  chassis.waitUntilDist(10);
  blocker.toggle();
  WAIT_UNTIL_DONE;
}

void farSideMid() { // does not work

  chassis.setPose(-40.5, -54, 180);

  // GAS GAS GAS !!!
  pros::Task task{[=] {
    pros::delay(1000);
    TOGGLE_WINGS;
  }};
  chassis.follow(farSideMidRush_txt, 3000, 12, false, false);

  // score the baddie in the intake
  intake = Intake::STATE::OUT;
  Rwingus.toggle();
  chassis.moveTo(-25, chassis.getPose().y - 10, -90, 1500);
  intake = Intake::STATE::IDLE;
  TurnToAngle(90, 1000);
  chassis.moveTo(-50, chassis.getPose().y, -90, 1000, false, false);
}

void skills() {
  chassis.setPose(-40.5, 54.5, 90);

  // push preload into goal

  // move to matchloader
  chassis.moveTo(-60, 44, 107, 1500, false, false);
  Lwingus.toggle();

  // shoot matchloads
  /*
  catapult.setState(balls::Catapult::State::Matchload);

  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }
  */

  pros::delay(1000);
  Lwingus.toggle();
  // push time !!
  TurnToAngle(-90, 1000);

  chassis.follow(skillspush_txt, 5400, 17, false, false, 90);

  chassis.moveTo(chassis.getPose().x - 20, chassis.getPose().y, -90, 1200);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  TurnToAngle(0, 500);
  pros::delay(100);

  chassis.moveTo(chassis.getPose().x, chassis.getPose().y - 12, 0, 500, false,
                 false);
  TurnToAngle(-90, 500);
  TOGGLE_WINGS;
  pros::delay(100);

  chassis.moveTo(chassis.getPose().x + 70, chassis.getPose().y, -90, 1600,
                 false, false);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  TOGGLE_WINGS;
  pros::delay(200);

  chassis.moveTo(chassis.getPose().x - 26, chassis.getPose().y, -90, 1500);
  TurnToAngle(0, 500);
  pros::delay(100);

  chassis.moveTo(chassis.getPose().x, chassis.getPose().y - 60, 0, 500, false,
                 false);
  TurnToAngle(-135, 500);
  TOGGLE_WINGS;
  pros::delay(100);
  chassis.moveTo(chassis.getPose().x + 70, chassis.getPose().y + 20, -90, 1600,
                 false, false, 0, .8);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;

  chassis.moveTo(chassis.getPose().x - 30, chassis.getPose().y, -90, 1600);
}