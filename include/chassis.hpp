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
};

} // namespace balls