#include "Solenoid.hpp"
#include "cata.hpp"
#include "chassis.hpp"
#include "intake.h"
#include "main.h"


sylib::Addrled intakeLED(22, 1, 28);

ryan::Solenoid doinker('F');
ryan::Solenoid LeftWingus('G');
ryan::Solenoid RightWingus('H');

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

pros::Motor cataMotor(5, false);
pros::Rotation cataRotation(6, false);

balls::Catapult cata(cataMotor, cataRotation, 54, 68.75);