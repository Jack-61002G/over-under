#include "autons.hpp"
#include "cata.hpp"
#include "intake.h"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"

ASSET(disruptSwap_txt)
ASSET(descore_txt)

ASSET(skillsPushOver_txt)
ASSET(skillsToOtherSide_txt)
ASSET(toFrontOfGoal_txt)
ASSET(loop_txt)

ASSET(skillspush_txt)
ASSET(reallyjankpush_txt)
ASSET(rush_txt)
ASSET(rushfinalpush_txt)
ASSET(kaboom_txt)

void holdAngle(double angle) {
  pros::Task task{[=] {
    const int kP = 7;
    const double kD = 12.0;
    const double drift = 10.0;

    double prevError = 0;

    while (catapult.getState() == balls::Catapult::State::Firing) {
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
    while (catapult.getState() == balls::Catapult::State::Firing) {
      chassis.moveToPose(x, y, theta, 500);
      chassis.waitUntilDone();
      pros::delay(50);
    }
  }};
}

void watchTheYaw(double secs) {
  pros::Task task{[=] {

    int startTime = pros::millis();
    while (pros::millis() - startTime < secs * 1000) {

      if (imu.get_roll() < -5) {
        chassis.cancelAllMotions();
        leftMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
        rightMotors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
        return;
      }
      pros::delay(100);
    }
  }};
}

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
  chassis.waitUntilDone();
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
  chassis.setPose(-46.25, -53, 0);
  Lwingus.set(true);
  pros::delay(150);

  // slap the triball out
  chassis.turnTo(-100, -55, 600);
  chassis.waitUntilDone();

  // turn back
  chassis.moveToPoint(-41, -53, 1000, false);
  chassis.waitUntilDone();
  turnToAngle(0, 100);
  Lwingus.set(false);
  chassis.turnTo(-8, -60, 1000);
  chassis.waitUntilDone();

  // touch bar
  chassis.moveToPose(-15.75, -60, 90, 2500, {true, 0, 0.5, 70});
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  pros::delay(1000);
  intake = Intake::STATE::IDLE;
}

void rush() {
  chassis.setPose(33.5, -53, 0);

  // rush first ball
  Rwingus.set(true); // kick preload
  intake = Intake::STATE::IN;
  chassis.follow(rush_txt, 15, 1700);
  chassis.waitUntil(6.8);
  Rwingus.set(false); // retract wing
  chassis.waitUntilDone();

  pros::delay(250);

  // back away
  chassis.moveToPoint(38.5, -53.5, 1500, false);

  // huck the ball at the side of the goal
  chassis.turnTo(55, -30, 500);
  pros::delay(100);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  // grab ball under elevation bar
  chassis.turnTo(0, -70, 600);
  chassis.moveToPoint(-2, -69, 2000);
  intake = Intake::STATE::IN;
  chassis.waitUntilDone();

  // move towards goal
  chassis.moveToPose(50.5, -41, 180, 2200, {false, 0, 0.3});
  chassis.waitUntil(23.5);
  Lwingus.set(true); // get ball from mathloader
  chassis.waitUntilDone();
  Lwingus.set(false);

  chassis.turnTo(chassis.getPose().x - 30, chassis.getPose().y, 150);
  chassis.turnTo(55, 0, 800);

  chassis.waitUntilDone();
  chassis.moveToPoint(55.5, 0, 700); // push in
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 1, chassis.getPose().y - 14, 500, false); // back out
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 1, chassis.getPose().y + 43, 750); // push again
  chassis.waitUntilDone();

  chassis.setPose(55, -34, 0);

  chassis.moveToPoint(39, -43, 1500, false);
  chassis.waitUntilDone();
  std::cout << chassis.getPose().x << "  " << chassis.getPose().y << "  "
            << chassis.getPose().theta << std::endl;
  intake = Intake::STATE::IDLE;

  chassis.turnTo(0, 0, 500);
  chassis.follow(rushfinalpush_txt, 14, 2600);

  watchTheYaw(6);

  chassis.waitUntil(30);
  intake = Intake::STATE::IN;

  chassis.waitUntil(50);
  intake = Intake::STATE::IDLE;

  chassis.waitUntil(60);
  Lwingus.set(true);
  Rwingus.set(true);

  chassis.waitUntil(75);
  intake = Intake::STATE::OUT;

  chassis.waitUntilDone();

  chassis.moveToPoint(chassis.getPose().x - 15, chassis.getPose().y, 1000,
                      false);
}

void disrupt() {

  chassis.setPose(-34, -53, 0);
  chassis.follow(disruptSwap_txt, 15, 1500);
  chassis.waitUntil(8.75);
  intake = Intake::STATE::IN;
  chassis.waitUntilDone();
  chassis.follow(kaboom_txt, 17, 1500, false);
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  // outtake balls into the alley
  chassis.turnTo(10, -60, 700);
  chassis.waitUntilDone();

  intake = Intake::STATE::OUT;

  pros::delay(400);
  intake = Intake::STATE::IDLE;

  Lwingus.set(true);
  pros::delay(150);

  // slap the triball out
  chassis.turnTo(-50, 50, 200);
  chassis.turnTo(-100, -55, 600);
  chassis.waitUntilDone();

  Lwingus.set(false);
  pros::delay(300);

  chassis.turnTo(-10, -62, 500);
  chassis.waitUntilDone();

  intake = Intake::STATE::OUT;

  chassis.moveToPoint(-8, -58, 1100, true, 90);
  chassis.waitUntilDone();

  chassis.moveToPoint(-50, -51, 1500, false, 90);
  intake = Intake::STATE::IDLE;
  chassis.waitUntilDone();

  chassis.turnTo(50, chassis.getPose().y, 100, true, 50);

}

void swap() {

  chassis.setPose(-34, -54, 0);
  Lwingus.set(true);
  chassis.follow(disruptSwap_txt, 15, 1500);
  chassis.waitUntil(8.75);
  Lwingus.set(false);
  intake = Intake::STATE::IN;
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x, -7, 500);
  chassis.waitUntilDone();
  chassis.turnTo(chassis.getPose().x + 30, -7, 500);
  chassis.waitUntilDone();
  Lwingus.set(true);
  intake = Intake::STATE::OUT;
  chassis.moveToPoint(0, -6, 1000, true);
  chassis.waitUntilDone();
  Lwingus.set(false);
  chassis.moveToPoint(-20, -9, 1000, false);
  pros::delay(200);
  intake = Intake::STATE::IDLE;
  chassis.waitUntilDone();

  chassis.turnTo(-60, -60, 800);
  chassis.waitUntilDone();
  chassis.moveToPoint(-52.7, -52.7, 1050);

  // chassis.follow(descore_txt, 11, 4000, true);
  // chassis.waitUntilDone();
}

void skills() {
  chassis.setPose(-47, 57.5, 225);

  // push preload into goal
  chassis.moveToPose(-58, -10, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-57.5, 37, -70, 1500, {false, 0, 0.2});
  chassis.waitUntilDone();

  Rwingus.toggle();
  pros::delay(100);

  catapult.setState(balls::Catapult::State::Firing);

  chassis.setPose(-55, 47, chassis.getPose().theta);
  holdAngle(290);
  pros::delay(22500);

  leftDriveLED.set_all(0x990000);
  rightDriveLED.set_all(0x990000);
  frontLED.set_all(0x990000);
  backLED.set_all(0x990000);
  pros::delay(1500);

  lights.setColor(leftDriveLED);
  lights.setColor(rightDriveLED);
  lights.setColor(frontLED);
  lights.setColor(backLED);
  catapult.setState(balls::Catapult::State::Idle);

  Rwingus.toggle();

  chassis.turnTo(-29, 55, 1000, false);
  chassis.waitUntilDone();
  chassis.follow(skillspush_txt, 15, 6000, false);
  chassis.waitUntilDone();
  
  // push
  chassis.turnTo(60, 2, 1000);
  chassis.waitUntilDone();
  Rwingus.set(true);
  chassis.moveToPoint(60, 10, 1000);

  chassis.waitUntil(24);
  Rwingus.set(false);
  chassis.waitUntilDone();

  //second push (no wings)
  chassis.moveToPoint(chassis.getPose().x-8, chassis.getPose().y, 1000, false);
  chassis.waitUntilDone();
  chassis.arcade(127, 0);
  pros::delay(700);
  chassis.arcade(0, 0);

  chassis.turnTo(100, chassis.getPose().y, 1000);
  chassis.waitUntilDone();

  // back out
  chassis.moveToPoint(12, 12, 800, false);
  chassis.waitUntilDone();

  // scooch over
  chassis.turnTo(chassis.getPose().x, -60, 1000);
  chassis.moveToPoint(chassis.getPose().x, 0, 1000);
  chassis.turnTo(60, 0, 1000);
  chassis.waitUntilDone();

  Lwingus.set(true);
  Rwingus.set(true);
  pros::delay(100);

  // second push
  chassis.moveToPose(65, 0, 90, 1200, {true, 0, 0.2, 127, 100});
  chassis.waitUntilDone();
  Rwingus.set(false);

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
  chassis.moveToPoint(11, 0, 800, false);

  // scooch over
  chassis.turnTo(12, -30, 1000);
  chassis.moveToPoint(12, -35, 1000);
  chassis.turnTo(60, 0, 1000);

  // third push
  chassis.moveToPose(60, -5, 90, 1500);
  Lwingus.set(true);
  chassis.waitUntilDone();

  //second push
  Lwingus.set(false);
  chassis.moveToPoint(chassis.getPose().x-8, chassis.getPose().y, 1000, false);
  chassis.waitUntilDone();
  chassis.arcade(127, 0);
  pros::delay(700);
  chassis.arcade(0, 0);

  // back out
  chassis.moveToPoint(7, chassis.getPose().y, 1200, false);
  chassis.waitUntilDone();

  // wall reset
  chassis.setPose(8, chassis.getPose().y, 90);

  // push into corner
  Lwingus.set(true);
  chassis.turnTo(60, -70, 800);
  chassis.waitUntilDone();
  chassis.moveToPoint(44, -53, 2500, true, 70);
  chassis.waitUntilDone();

  Lwingus.set(false);
  Rwingus.set(true);

  // funnel balls with R wing
  chassis.turnTo(100, chassis.getPose().y + 10, 800);
  chassis.waitUntilDone();
  Rwingus.set(false);
  pros::delay(400);
  
  // push
  chassis.moveToPose(60, 0, 0, 1500);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  // get out
  chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y - 10, 1200, false);
  chassis.waitUntilDone();

  chassis.moveToPoint(63.5, chassis.getPose().y + 50, 1200);
  chassis.waitUntilDone();

  chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y - 12, 1200, false);
  chassis.waitUntilDone();

  chassis.moveToPoint(63.5, chassis.getPose().y + 50, 1200);
  chassis.waitUntilDone();

  chassis.moveToPoint(63.5, chassis.getPose().y - 16, 1200, false);
}

void skillsNew() {
  chassis.setPose(-47, 57.5, 225);

  // push preload into goal
  chassis.moveToPose(-57.5, 0, 180, 1200);
  chassis.waitUntil(10);
  intake = Intake::STATE::OUT;
  chassis.waitUntilDone();

  intake = Intake::STATE::IDLE;

  pros::delay(200);

  // move to matchloader
  chassis.moveToPose(-57, 37, -65, 1500, {false, 0, 0.2});
  chassis.waitUntilDone();

  Rwingus.toggle();

  catapult.setState(balls::Catapult::State::Firing);

  chassis.setPose(-55, 47, chassis.getPose().theta);
  holdAngle(290);
  pros::delay(22000);

  leftDriveLED.set_all(0x990000);
  rightDriveLED.set_all(0x990000);
  frontLED.set_all(0x990000);
  backLED.set_all(0x990000);
  pros::delay(1500);

  lights.setColor(leftDriveLED);
  lights.setColor(rightDriveLED);
  lights.setColor(frontLED);
  lights.setColor(backLED);
  catapult.setState(balls::Catapult::State::Idle);

  Rwingus.toggle();

  pros::delay(500);

  chassis.turnTo(-20, 38, 1000, true);
  chassis.waitUntilDone();
  chassis.follow(skillsPushOver_txt, 15, 3100, true);
  intake = Intake::STATE::OUT;

  chassis.waitUntil(36);
  Rwingus.set(true);

  chassis.waitUntilDone();
  chassis.setPose(-10, chassis.getPose().y, chassis.getPose().theta);
  Rwingus.set(false);
  pros::delay(100);

  chassis.turnTo(chassis.getPose().x-90, chassis.getPose().y-2, 800);
  chassis.waitUntilDone();

  chassis.moveToPoint(-48, -48, 1000);
  chassis.waitUntilDone();
  chassis.turnTo(-35, -60, 800);
  chassis.waitUntilDone();
  chassis.follow(skillsToOtherSide_txt, 15, 3800);
  chassis.waitUntilDone();

  // push into side of goal
  intake = Intake::STATE::OUT;
  chassis.moveToPoint(chassis.getPose().x + 0.25, chassis.getPose().y+30, 800, true);
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 0.25, chassis.getPose().y-15, 350, false);
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 0.25, chassis.getPose().y+40, 1000, true);
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 0.25, chassis.getPose().y-15, 350, false);
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x + 0.25, chassis.getPose().y+40, 1000, true);
  chassis.waitUntilDone();
  intake = Intake::STATE::IDLE;

  // position reset against the goal
  chassis.setPose(59.5, -30.5, chassis.getPose().theta);
  pros::delay(100);
  chassis.follow(toFrontOfGoal_txt, 15, 1800, false);
  chassis.waitUntilDone();
  chassis.turnTo(70, 0, 800);
  
  // first push from front
  pros::delay(100);
  chassis.moveToPose(70, -13, 90, 1000, {true, 15, .6, 127, 127});
  chassis.waitUntilDone();

  chassis.follow(loop_txt, 10, 3000, false);

  //last 2 front pushes
  chassis.turnTo(60, 0, 1000);
  chassis.waitUntilDone();
  chassis.moveToPose(60, 14, 90, 1000);
  chassis.waitUntilDone();

  // back out
  chassis.moveToPoint(0, 9, 1000, false);
  chassis.waitUntilDone();


  Lwingus.set(true);
  Rwingus.set(true);

  // second push
  chassis.arcade(127, 0);
  pros::delay(800);
  chassis.arcade(0, 0);
  pros::delay(100);
  chassis.arcade(-127, 0);
  pros::delay(500);
  chassis.arcade(0, 0);
  pros::delay(100);
  chassis.arcade(127, 0);
  pros::delay(800);


  chassis.tank(-100, -100);
  pros::delay(500);
  chassis.tank(0, 0);
  pros::delay(300);



}