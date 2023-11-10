#include "lemlib/asset.hpp"
#include "robotconfig.h"
#include <iostream>

ASSET(skillspush_text)

void closeSide3() {
  chassis.setPose(-17, 59.5, 90);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 59.5, 90, 1000);
  chassis.moveTo(-64, 20, 0, 2350, false, false, 13, .65, 110);
  chassis.setPose(-64, 29.625, 0);
  chassis.moveTo(-60, 34, 0, 1000);
  chassis.turnTo(chassis.getPose().x, -60, 500);
  intake = Intake::STATE::OUT;
  chassis.moveTo(chassis.getPose().x, 10, 180, 650);
  intake = Intake::STATE::IDLE;
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  chassis.moveTo(-50, 45, -135, 800, false, false);
  intake = Intake::STATE::IN;
  chassis.moveTo(0, 21, 90, 1500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  chassis.turnTo(-55, -50, 750);
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-60, -40, 180, 1500);
}

void closeSideMid() { // this shawty does NOT work

  chassis.setPose(-40.75, 55, 180);

  // drive into top mid ball with intake on
  intake = Intake::STATE::IN;
  chassis.moveTo(-25, -3, 180, 1400);

  // turn to face goal and outtake
  chassis.turnTo(-65, 4, 325);
  pros::delay(150);
  intake = Intake::STATE::OUT;
  pros::delay(50);

  // turn and drive to bottom mid ball
  chassis.turnTo(-4, 0, 250);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 0, 90, 750);
  pros::delay(50);

  // turn to face goal and push both into goal
  chassis.turnTo(-65, 17, 400);
  pros::delay(50);
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-73, 4, -90, 1000);
  pros::delay(500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;

  // turn and drive to bottom right side ball
  chassis.moveTo(-38, 0, 0, 650, false, false);
  intake = Intake::STATE::IN;
  chassis.moveTo(-22, 19, 90, 1300);
  pros::delay(150);
  intake = Intake::STATE::HOLD;
  pros::delay(500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;

  // back up to goal and outtake it to the right of the goal
  chassis.moveTo(-40, 20, 110, 1000, false, false);
  pros::delay(500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  chassis.moveTo(-42, 40, 150, 1000);
  pros::delay(500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;
  chassis.turnTo(-100, 38, 750);
  pros::delay(100);
  intake = Intake::STATE::OUT;
  pros::delay(150);
  pros::delay(500);
  std::cout << chassis.getPose().x << " " << chassis.getPose().y << std::endl;

  // get the side bar ball and intake it
  chassis.turnTo(-40, 100, 500);
  intake = Intake::STATE::IN;
  chassis.moveTo(6, 50, 90, 5000, false, true, 13, .9);

  // back up to the matchload zone
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-64, 20, 0, 2350, false, false, 13, .65, 110);

  // doinker the triball
  chassis.moveTo(-48, 48, -90, 1000, false, false);
  doinker.toggle();
  chassis.turnTo(-60, 22, 1000);
  doinker.toggle();

  // push all 3 in
  intake = Intake::STATE::OUT;
  chassis.moveTo(-60, 10, 180, 1500);
}

void farSide() {
  chassis.setPose(-45, -54.25, 180);

  // aim to slap the triball out
  chassis.turnTo(-60, -70, 750);

  // slap the triball out
  doinker.toggle();
  pros::delay(500);

  chassis.turnTo(-30, -70, 750);
  doinker.toggle();

  // score preload in goal
  chassis.moveTo(-59, 5, 180, 1500, false, false, 0, 0.75);
  pros::delay(500);

  // wall reset on goal!
  chassis.setPose(-70, -30, chassis.getPose().theta);

  // move that jawn to the elevation bar and knock those triballs to the other
  // side
  chassis.moveTo(4, -57, 90, 3000, false, true, 0, 0.85);
  doinker.toggle();
}

void farSideMid() { // need a far side mid auton still

  

}

void skills() {

  // push preload into goal

  // move to matchloader

  // shoot matchloads
  catapult.setState(balls::Catapult::State::Matchload);

  while (catapult.getState() == balls::Catapult::State::Matchload) {
    pros::delay(20);
  }

  // push time !!
  chassis.follow(skillspush_text, 10000, 10);
}