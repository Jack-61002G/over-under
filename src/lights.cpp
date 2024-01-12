#include "lights.hpp"
#include "autoSelect/selection.h"
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
  leftDriveLED.cycle(*leftDriveLED, 5);

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

  enum class State { Disabled, Driver, Auto};
  State gameState = State::Disabled;

  bool Lwing = false;
  bool Rwing = false;

  int intakeVal = 0;

  while (true) {

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

    if (!pros::competition::is_disabled() && !pros::competition::is_autonomous() && gameState != State::Driver) {
      setColor(underglowLED);
      setColor(backLED);
      setColor(leftDriveLED);
      setColor(rightDriveLED);
      gameState = State::Driver;
    }

    if ()
    
    pros::delay(18);
  }
}