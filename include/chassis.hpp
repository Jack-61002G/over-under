#include "lemlib/api.hpp"
#include "main.h"
#include "preplanning.h"
#include "trapevel.h"
#include <memory>

namespace balls {

class chassis : public lemlib::Chassis {

private:
  std::shared_ptr<trapevel> leftController = nullptr;
  std::shared_ptr<trapevel> rightController = nullptr;

public:
  // super constructor
  chassis(lemlib::Drivetrain_t drivetrain,
          lemlib::ChassisController_t lateralSettings,
          lemlib::ChassisController_t angularSettings,
          lemlib::OdomSensors_t sensors)
      : lemlib::Chassis(drivetrain, lateralSettings, angularSettings, sensors) {
  }

  void VelController(char side, double k_UP, double k_DOWN, double k_VEL,
                     double kP) {

    if (side == 'l') {
      leftController = std::make_shared<trapevel>(k_UP, k_DOWN, k_VEL, kP);
    } else if (side == 'r') {
      rightController = std::make_shared<trapevel>(k_UP, k_DOWN, k_VEL, kP);
    }
  }

  void followProfile(trajectory path) {

    int i = 0;

    while (i < path.points.size()) {

      double leftVel =
          path.targetLinearVelocity[i] +
          path.targetAngularVelocity[i] * drivetrain.trackWidth / 2.0;
      double rightVel =
          path.targetLinearVelocity[i] -
          path.targetAngularVelocity[i] * drivetrain.trackWidth / 2.0;

      drivetrain.leftMotors->move(leftController->update(
          leftVel, drivetrain.leftMotors->get_actual_velocities()[0]));
      drivetrain.rightMotors->move(rightController->update(
          rightVel, drivetrain.rightMotors->get_actual_velocities()[0]));

      i++;
      pros::delay(20);
    }
    drivetrain.leftMotors->brake();
    drivetrain.rightMotors->brake();
  }

  // pilons joystick curve
  int curveJoystick(bool red, int input, double t) {

    // scale input from -127 to 127 to -100 to 100
    input = input * 100 / 127;

    int val = 0;
    if (red) {
      val = (std::exp(-t / 10) +
             std::exp((std::abs(input) - 100) / 10) * (1 - std::exp(-t / 10))) *
            input;
    } else {
      // blue
      val = std::exp(((std::abs(input) - 100) * t) / 1000) * input;
    }
    //scale back to 127 to -127
    return val * 127 / 100;
  }

  //arcade control
  void arcade(int forward, int turn) {

    //curve input
    forward = curveJoystick(true, forward, 7);
    turn = curveJoystick(false, turn, 7);

    // calculate left and right velocities
    double leftVel = forward + turn;
    double rightVel = forward - turn;

    // set the motors
    drivetrain.leftMotors->move(leftVel);
    drivetrain.rightMotors->move(rightVel);
  }
};

} // namespace balls