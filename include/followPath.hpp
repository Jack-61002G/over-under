#include "main.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <memory>
#include "preplanning.h"

void follow2d(trajectory path) {

  int i = 0;

  while (i < path.points.size()) {

    double leftVel = path.targetLinearVelocity[i] +
                     path.targetAngularVelocity[i] * 10.375 / 2.0;
    double rightVel = path.targetLinearVelocity[i] -
                      path.targetAngularVelocity[i] * 10.375 / 2.0;

    std::cout<<leftVel<<" "<<rightVel<<std::endl;
    

    leftMotors = leftController->update(leftVel, leftMotors.get_actual_velocities()[0]);
    rightMotors = rightController->update(rightVel, rightMotors.get_actual_velocities()[0]);
    pros::delay(25);
    i++;
  }
  leftMotors = 0;
  rightMotors = 0;
  leftMotors.brake();
  rightMotors.brake();
  leftController->reset();
    rightController->reset();
}
