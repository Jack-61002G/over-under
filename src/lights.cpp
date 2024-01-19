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

/*
void Lights::flow(sylib::Addrled &strip) {
  if (auton > 0) {
    strip.gradient(0xFF0000, 0x000000, 0, 0);
    strip.gradient(0x000000, 0xFF0000, 0, 18);
  }
  else if (auton < 0) {
    strip.gradient(0x0000FF, 0x000000, 0, 0);
    strip.gradient(0x000000, 0x0000FF, 0, 18);
  }
  else {
    strip.gradient(0xBB00BB, 0x000000, 0, 0);
    strip.gradient(0x000000, 0xBB00BB, 0, 18);
  }
  strip.cycle(*strip, 5);
}
*/

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

  enum class State { Disabled, Driver, Auto, RAINBOW};
  State gameState = State::Disabled;

  bool Lwing = false;
  bool Rwing = false;

  bool intakeVal = false;

  int driverTime = 0;

  while (true) {
    int time = pros::millis() - driverTime;

    if (gameState == State::RAINBOW) {
      pros::delay(100);
      continue;
    }

    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      rotate();
      gameState = State::Disabled;
    }

    if (!pros::competition::is_disabled() && pros::competition::is_autonomous() && gameState != State::Auto) {
      setColor(underglowLED);
      setColor(backLED);
      setColor(leftDriveLED);
      setColor(rightDriveLED);
      gameState = State::Auto;
    }

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

      if (time > 30000 && time < 30700) {
        underglowLED.set_all(0x00FF00);
        backLED.set_all(0x00FF00);
        leftDriveLED.set_all(0x00FF00);
        rightDriveLED.set_all(0x00FF00);
      }
      else if (time > 45000 && time < 45700) {
        underglowLED.set_all(0xAA8800);
        backLED.set_all(0xAA8800);
        leftDriveLED.set_all(0xAA8800);
        rightDriveLED.set_all(0xAA8800);
      }
      else if (time > 55000) {
        gameState = State::RAINBOW;
        underglowLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
        underglowLED.cycle(*underglowLED, 5);
        backLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
        backLED.cycle(*backLED, 5);
        leftDriveLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
        leftDriveLED.cycle(*leftDriveLED, 5);
        rightDriveLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
        rightDriveLED.cycle(*rightDriveLED, 5);
      }
      else {
        setColor(underglowLED);
        setColor(backLED);
        setColor(leftDriveLED);
        setColor(rightDriveLED);
      }
    }
    
    pros::delay(100);
  }
}