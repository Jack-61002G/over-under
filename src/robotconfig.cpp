#include "Solenoid.hpp"
#include "cata.hpp"
#include "EZ-Template/util.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "main.h"
#include "pros/motors.hpp"
#include "lights.hpp"
#include <iostream>

sylib::Addrled intakeLED(22, 2, 28);
sylib::Addrled doinkerLED(22, 8, 32);

ryan::Solenoid doinker('G');

// Chassis constructor
Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    {-6, -5, 7}


    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    ,
    {10, 9, -8}

    // IMU Port
    ,
    20

    // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
    //    (or tracking wheel diameter)
    ,
    3.25

    // Cartridge RPM
    //   (or tick per rotation if using tracking wheels)
    ,
    600

    // External Gear Ratio (MUST BE DECIMAL)
    //    (or gear ratio of tracking wheel)
    // eg. if your drive is 84:36 where the 36t is powered, your RATIO would
    // be 2.333. eg. if your drive is 36:60 where the 60t is powered, your RATIO
    // would be 0.6.
    ,
    1.666

);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor cataMotor(3, false);
pros::Rotation cataRotation(19, false);

balls::Catapult catapult(cataMotor, cataRotation, 66);

pros::Motor intakeMotor(2, true);

Intake intake(intakeMotor);

ryan::Solenoid blocker('E');

balls::Lights lights(intakeLED, doinkerLED, doinker);