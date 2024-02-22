#include "lights.hpp"
#include "autoSelect/selection.h"
#include "display/lv_objx/lv_gauge.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace balls;


void Lights::rotate() {

  // intakeLED.clear();
  underglowLED.clear();
  backLED.clear();

  leftDriveLED.clear();
  leftDriveLED.set_pixel(0x600060, 0);
  leftDriveLED.set_pixel(0x990099, 1);
  leftDriveLED.set_pixel(0x990099, 2);
  leftDriveLED.set_pixel(0x600060, 3);
  leftDriveLED.cycle(*leftDriveLED, 5, 0, true);

  rightDriveLED.clear();
  rightDriveLED.set_pixel(0x600060, 0);
  rightDriveLED.set_pixel(0x990099, 1);
  rightDriveLED.set_pixel(0x990099, 2);
  rightDriveLED.set_pixel(0x600060, 3);
  rightDriveLED.cycle(*rightDriveLED, 5);
}


void Lights::setColor(sylib::Addrled &strip) {
  
  if (auton > 0) {
    strip.set_all(0x990000);
    
  } else if (auton < 0) {
    strip.set_all(0x000099);

  } else {
    strip.set_all(0x600060);
  }
}


void Lights::loop() {

  enum class State { Disabled, Driver, Auto };
  State gameState = State::Disabled;

  bool Lwing = false;
  bool Rwing = false;

  bool intakeVal = false;

  int driverTime = 0;

  while (true) {
    int time = pros::millis() - driverTime;

    // disabled
    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      rotate();
      gameState = State::Disabled;
    }

    // autonomous
    if (!pros::competition::is_disabled() && pros::competition::is_autonomous() && gameState != State::Auto) {
      setColor(underglowLED);
      setColor(backLED);
      setColor(leftDriveLED);
      setColor(rightDriveLED);
      gameState = State::Auto;
    }

    // driver control
    if (!pros::competition::is_disabled() && !pros::competition::is_autonomous()) {
      if (gameState != State::Driver) {
        setColor(underglowLED);
        setColor(backLED);
        setColor(leftDriveLED);
        setColor(rightDriveLED);
        gameState = State::Driver;
        if (auton == 0) {
          driverTime = pros::millis();
        } else {
          driverTime = pros::millis() + 45000;
        }
      }

      if (time > 30000 && time < 30700 && auton != 0) {
        underglowLED.set_all(0x00AA00);
        backLED.set_all(0x00AA00);
        leftDriveLED.set_all(0x00AA00);
        rightDriveLED.set_all(0x00AA00);
      }
      else if (time > 45000 && time < 45700) {
        underglowLED.set_all(0xFFD800);
        backLED.set_all(0xFFD800);
        leftDriveLED.set_all(0xFFD800);
        rightDriveLED.set_all(0xFFD800);
      }
      else if (time > 45000 || auton != 0) {
        setColor(underglowLED);
        setColor(backLED);
        setColor(leftDriveLED);
        setColor(rightDriveLED);
      }
    }
    
    pros::delay(120);
  }
}