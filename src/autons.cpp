#include "lemlib/asset.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <iostream>

ASSET(skillspush_txt)
ASSET(sidepush_txt)

void closeSide3() {
  chassis.setPose(-17, 59.5, 90);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 59.5, 90, 1000);
  chassis.moveTo(-64, 20, 0, 2500, false, false, 13, .65, 110);
  chassis.setPose(-64, 29.625, 0);
  chassis.moveTo(-60, 34, 0, 1000);
  chassis.turnTo(chassis.getPose().x, -60, 500);
  intake = Intake::STATE::OUT;
  chassis.moveTo(chassis.getPose().x, 10, 180, 650);
  intake = Intake::STATE::IDLE;
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  chassis.moveTo(-50, 45, -135, 1000, false, false);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 25, 90, 1500);
  chassis.moveTo(-10, 25, 90, 500, false, false);
  chassis.turnTo(-55, -50, 750);
  intake = Intake::STATE::HOLD;
  chassis.moveTo(-70, 7, -90, 1000);
  chassis.moveTo(-40, 7, -90, 1000, false, false);
}

void closeSideMid() { // this shawty does NOT work
  closeSide3();
}

void farSide() {
  chassis.setPose(-45, -54.25, 180);

  // aim to slap the triball out
  chassis.turnTo(-60, -70, 750);

  // slap the triball out
  //doinker.toggle();
  pros::delay(500);

  chassis.turnTo(-30, -70, 750);
  //doinker.toggle();

  // score preload in goal
  chassis.moveTo(-59, -12, 180, 1500, false, false, 0, 0.75);
  pros::delay(500);

  // wall reset on goal!
  //chassis.setPose(-70, -30, chassis.getPose().theta);

  // move that jawn to the elevation bar and knock those triballs to the other
  // side
  chassis.moveTo(4, -57, 90, 3000, false, true, 0, 0.85);
  //doinker.toggle();
}

void farSideMid() { // need a far side mid auton still
}

void skills() {
  chassis.setPose(-40.5, 54.5, -90);

  // push preload into goal

  // move to matchloader
  chassis.moveTo(-54, 48.2, -71, 1000);
  //doinker.toggle();

  // shoot matchloads
  catapult.setState(balls::Catapult::State::Matchload);

  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }
  //doinker.toggle();
  // push time !!
  chassis.turnTo(-30, 60, 1000);
  chassis.follow(skillspush_txt, 5300, 17);
  chassis.tank(127, 127);
  pros::delay(500);
  chassis.tank(0,0);
  chassis.setPose(42, -7, 90);
  chassis.moveTo(0, 7, 90, 1500, false, false);
  chassis.moveTo(55, 7, 90, 800);
  chassis.tank(127, 127);
  pros::delay(500);
  chassis.tank(0,0);
  chassis.moveTo(-20, 7, 90, 1000, false, false);
  pros::delay(250);
  chassis.setPose(8, 7, 90);
  chassis.follow(sidepush_txt, 4000, 15);
  chassis.tank(127, 127);
  pros::delay(500);
  chassis.tank(0,0);
}