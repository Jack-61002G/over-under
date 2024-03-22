#include "lights.hpp"
#include "autoSelect/selection.h"
#include "display/lv_objx/lv_gauge.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

using namespace balls;

// Set the color of a strip to our alliance color
void Lights::setColor(sylib::Addrled &strip) {

  if (selector::auton > 0) {
    strip.set_all(0x990000);

  } else if (selector::auton < 0) {
    strip.set_all(0x000099);

  } else {
    strip.set_all(0x600060);
  }
}

void Lights::loop() {

  enum class State { Start, Disabled, Enabled };
  State gameState = State::Start;

  int timeEnabled = 0;
  int heatlevel = 0;   // 0, 1, 2
  int timewarning = 0; // 0, 1, 2

  while (true) {

    // disabled
    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      gameState = State::Disabled;

      frontLED.set_all(0x600060);
      backLED.set_all(0x600060);
      leftWingLED.clear();
      rightWingLED.clear();

      leftWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
      leftWingLED.cycle(*leftWingLED, 5, 0);
      rightWingLED.gradient(0x000000, 0xFF00FF, 3);
      rightWingLED.cycle(*rightWingLED, 5);
    }


    // enabled
    if (!pros::competition::is_disabled()) {

      // first time entering enabled
      if (gameState != State::Enabled) {
        gameState = State::Enabled;

        timeEnabled = pros::millis();
      }

      // color changing for time warnings
      if (75000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 75700 and timewarning != 1) {
        timewarning = 1;
        frontLED.set_all(0x00AA00);
        backLED.set_all(0x00AA00);
        leftWingLED.set_all(0x00AA00);
        rightWingLED.set_all(0x00AA00);

      } else if (90000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 90700 and timewarning != 2) {
        timewarning = 2;
        frontLED.set_all(0xFFD800);
        backLED.set_all(0xFFD800);
        leftWingLED.set_all(0xFFD800);
        rightWingLED.set_all(0xFFD800);
        
      } else if (timewarning != 0) {
        timewarning = 0;
        setColor(frontLED);
        setColor(backLED);
        setColor(leftWingLED);
        setColor(rightWingLED);
      }

      // motor temperature display
      if (timewarning == 0) {
        int temp = std::max({leftFront.get_temperature(), leftBack.get_temperature(), leftTop.get_temperature(), rightFront.get_temperature(), rightBack.get_temperature(), rightTop.get_temperature()});

        if (temp >= 40 and temp < 50 and heatlevel != 1) {
          heatlevel = 1;
          leftWingLED.gradient(0x000000, 0x990099, 0, 1);
          leftWingLED.cycle(*leftWingLED, 3, 0);
          rightWingLED.gradient(0x000000, 0x990099, 0, 1);
          rightWingLED.cycle(*rightWingLED, 3);
        }
        else if (temp > 55 and heatlevel != 2) {
          heatlevel = 2;
          leftWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
          leftWingLED.cycle(*leftWingLED, 6, 0);
          rightWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
          rightWingLED.cycle(*rightWingLED, 6);
        }
        else if (temp < 40 and heatlevel != 0) {
          heatlevel = 0;
          setColor(leftWingLED);
          setColor(rightWingLED);
        }
      }
    }



/*

    // motor temperature display
    if (gameState == State::Custom) {
      
    }

    // autonomous
    if (!pros::competition::is_disabled() &&
        pros::competition::is_autonomous() && gameState != State::Auto) {
      gameState = State::Auto;
      setColor(leftWingLED);
      setColor(rightWingLED);
      setColor(frontLED);
      setColor(backLED);
    }

    // driver control
    if (!pros::competition::is_disabled() &&
        !pros::competition::is_autonomous()) {
      if (gameState != State::Driver) {
        setColor(leftWingLED);
        setColor(rightWingLED);
        setColor(frontLED);
        setColor(backLED);
        gameState = State::Driver;
        if (selector::auton == 0) {
          driverTime = pros::millis();
        } else {
          driverTime = pros::millis() + 45000;
        }
      }

      if (time > 30000 && time < 30700 && selector::auton != 0) {
        leftWingLED.set_all(0x00AA00);
        rightWingLED.set_all(0x00AA00);
        frontLED.set_all(0x00AA00);
        backLED.set_all(0x00AA00);
      } else if (time > 45000 && time < 45700) {
        leftWingLED.set_all(0xFFD800);
        rightWingLED.set_all(0xFFD800);
        frontLED.set_all(0xFFD800);
        backLED.set_all(0xFFD800);
      } else if (time > 45000 || selector::auton != 0) {
        setColor(leftWingLED);
        setColor(rightWingLED);
        setColor(frontLED);
        setColor(backLED);
      }
    }

    // 10ms delay to allow other tasks to run
    pros::delay(30);
    */
  }
}