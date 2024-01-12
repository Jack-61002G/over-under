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

void Lights::flash(sylib::Addrled &strip) {
  pros::Task task{[=] {

    if (auton > 0) {
      strip.set_all(0xBB0000);
    }
    else if (auton < 0) {
      strip.set_all(0x0000BB);
    }
    else {
      strip.set_all(0x800080);
    }

    pros::delay(150);

    strip.clear();

    pros::delay(150);

    setColor(strip);

  }};
}


void Lights::loop() {

  enum class State { Disabled, Driver, Auto};
  State gameState = State::Disabled;

  bool Lwing = false;
  bool Rwing = false;

  bool intakeVal = false;

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

    if (Lwingus.getState() && !Lwing) {
      flash(leftDriveLED);
      Lwing = true;
    } else if (!Lwingus.getState()) {
      Lwing = false;
    }

    if (Rwingus.getState() && !Rwing) {
      flash(rightDriveLED);
      Rwing = true;
    } else if (!Rwingus.getState()) {
      Rwing = false;
    }

    if (intake.getState() != Intake::STATE::IDLE && !intakeVal) {
      int color;
      if (auton > 0) {color = 0xBB0000;}
      else if (auton < 0) {color = 0x0000BB;}
      else {color = 0x800080;}

      for (int i = 0; i < 28; i++) {
        if (i % 2 == 0) {
          underglowLED.set_pixel(color, i);
        } else {
          underglowLED.set_pixel(0, i);
        }
      }
      underglowLED.cycle(*underglowLED, 5);

      intakeVal = true;
    } else if (intake.getState() == Intake::STATE::IDLE) {
      intakeVal = false;
    }
    
    pros::delay(100);
  }
}