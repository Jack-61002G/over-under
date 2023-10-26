#include "EZ-Template/util.hpp"
#include "Solenoid.hpp"
#include "cata.hpp"
#include "graphy/Grapher.hpp"
#include "intake.h"
#include "lights.hpp"
#include "main.h"
#include "pros/motors.hpp"
#include <iostream>


sylib::Addrled underglowLED(22, 1, 28);
sylib::Addrled intakeLED(22, 2, 22);
sylib::Addrled doinkerLED(22, 5, 32);

ryan::Solenoid doinker('C');

// Chassis constructor
Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    {-11, -20, 10}

    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    ,
    {8, 7, -2}

    // IMU Port
    ,
    9

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
pros::Rotation cataRotation(1, false);

balls::Catapult catapult(cataMotor, cataRotation, 25);

pros::Motor intakeMotor(19, true);

Intake intake(intakeMotor);

ryan::Solenoid blocker('D');

ryan::Solenoid hang('F');

balls::Lights lights(underglowLED, doinkerLED, intakeLED, doinker);