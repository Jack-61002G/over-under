#include "main.h"
#include "chassis.hpp"

pros::Motor left_front_motor(3, true);    // port 1, not reversed
pros::Motor left_back_motor(2, true);     // port 2, not reversed
pros::Motor left_top_motor(1, false);     // port 3, reversed
pros::Motor right_top_motor(8, true);     // port 3, reversed
pros::Motor right_back_motor(9, false);   // port 4, reversed
pros::Motor right_front_motor(10, false); // port 2, not reversed

pros::MotorGroup left_side_motors({left_front_motor, left_back_motor,
                                   left_top_motor});
pros::MotorGroup right_side_motors({right_front_motor, right_back_motor,
                                    right_top_motor});

lemlib::Drivetrain_t drivetrain{
    &left_side_motors,          // left drivetrain motors
    &right_side_motors,         // right drivetrain motors
    10,                         // track width
    lemlib::Omniwheel::NEW_325, // wheel diameter
    360                         // wheel rpm
};

// inertial sensor
pros::Imu inertial_sensor(11); // port 2

pros::ADIEncoder back_encoder('G', 'H', true);
lemlib::TrackingWheel back =
    lemlib::TrackingWheel(&back_encoder, lemlib::Omniwheel::NEW_275, 5.5, 1);

// odometry struct
lemlib::OdomSensors_t sensors{
    nullptr, // vertical tracking wheel 1
    nullptr, // horizontal tracking wheel 1
    &back,   // vertical tracking wheel 2
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &inertial_sensor // inertial sensor
};

// forward/backward PID
lemlib::ChassisController_t lateralController{
    14,  // kP
    21,  // kD
    1,   // smallErrorRange
    100, // smallErrorTimeout
    3,   // largeErrorRange
    500, // largeErrorTimeout
    9    // slew rate
};

// turning PID
lemlib::ChassisController_t angularController{
    2,   // kP
    26,  // kD
    1,   // smallErrorRange
    50,  // smallErrorTimeout
    3,   // largeErrorRange
    200, // largeErrorTimeout
    0    // slew rate
};

// create the chassis
balls::chassis chassis(drivetrain, lateralController, angularController,
                       sensors);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

void screen() {
  // loop forever
  while (true) {
    lemlib::Pose pose =
        chassis.getPose(); // get the current position of the robot
    pros::lcd::print(0, "x: %f", pose.x);           // print the x position
    pros::lcd::print(1, "y: %f", pose.y);           // print the y position
    pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
    pros::delay(10);
  }
}

void initialize() {

  pros::lcd::initialize();
  chassis.calibrate();
  pros::Task screenTask(
      screen); // create a task to print the position to the screen
}

void disabled() {}

void competition_initialize() {}

void autonomous() { chassis.moveTo(12, 12, 0, 1000); }

void opcontrol() {
/*
  chassis.VelController('l', 2.2, 1.7, 23, 7.5);
  chassis.VelController('r', 2.2, 1.7, 23, 7.5);
  trajectory nya(Pose(0, 0, 0), Pose(11.59, 50, 1.2), 10, 27, 5, 5, 5.1, 1);
  chassis.followProfile(nya);
*/while (true) {


  chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
}}
