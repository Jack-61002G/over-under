#include "robotconfig.h"

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

void closeSideMid() {

    chassis.setPose(-40.75, 55, 180);

  // drive into top mid ball with intake on
  intake = Intake::STATE::IN;
  chassis.moveTo(-25, 0, 180, 1700);

  // turn to face goal and outtake
  chassis.turnTo(-65, 8, 350);
  intake = Intake::STATE::OUT;

  // turn and drive to bottom mid ball
  chassis.turnTo(-4, 0, 250);
  intake = Intake::STATE::IN;
  chassis.moveTo(-9, 0, 90, 500);

  // turn to face goal and push both into goal
  chassis.turnTo(-65, 17, 500);
  intake = Intake::STATE::IDLE;
  chassis.moveTo(-60, 12, -90, 800);

  // turn and drive to bottom right side ball
  chassis.moveTo(-34, -4, 0, 650, false, false);
  intake = Intake::STATE::IN;
  chassis.moveTo(-8, 24, 90, 1500);

  // back up to goal and outtake it to the right of the goal
  chassis.moveTo(-40, 40, 135, 1500, false, false);
  chassis.turnTo(-100, 38, 750);
  intake = Intake::STATE::OUT;

  // get the side bar ball and intake it
  chassis.turnTo(-40, 100, 500);
  intake = Intake::STATE::IN;
  chassis.moveTo(-6, 59.5, 90, 2000, false, true, 13, .9);

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