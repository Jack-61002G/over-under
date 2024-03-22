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

  enum class State { Start, Disabled, Driver, Auto, Custom };
  State gameState = State::Start;

  int driverTime = 0;
  int heatlevel = 0; // 0, 1, 2

  while (true) {
    int time = pros::millis() - driverTime;

    // motor temperature display
    if (gameState == State::Custom) {
      int temp = std::max({leftFront.get_temperature(), leftBack.get_temperature(), leftTop.get_temperature(), rightFront.get_temperature(), rightBack.get_temperature(), rightTop.get_temperature()});
      
      if (temp < 40 and heatlevel != 0) {
        heatlevel = 0;
        leftWingLED.clear();
        rightWingLED.clear();
      }
      if (temp >= 40 and temp < 50 and heatlevel != 1) {
        heatlevel = 1;
        leftWingLED.gradient(0x000000, 0x990099, 0, 1);
        leftWingLED.cycle(*leftWingLED, 3, 0);
        rightWingLED.gradient(0x000000, 0x990099, 0, 1);
        rightWingLED.cycle(*rightWingLED, 3);
      }
      if (temp > 55 and heatlevel != 2) {
        heatlevel = 2;
        leftWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
        leftWingLED.cycle(*leftWingLED, 6, 0);
        rightWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
        rightWingLED.cycle(*rightWingLED, 6);
      }
    }

    // disabled
    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      gameState = State::Disabled;
      frontLED.clear();
      backLED.clear();
      leftWingLED.clear();
      rightWingLED.clear();
      leftWingLED.gradient(0x000000, 0xFF00FF);
      leftWingLED.cycle(*leftWingLED, 5, 0);
      rightWingLED.gradient(0x000000, 0xFF00FF);
      rightWingLED.cycle(*rightWingLED, 5);
    }

    // autonomous
    if (!pros::competition::is_disabled() && pros::competition::is_autonomous() && gameState != State::Auto) {
      gameState = State::Auto;
      setColor(leftWingLED);
      setColor(rightWingLED);
      setColor(frontLED);
      setColor(backLED);
    }

    // driver control
    if (!pros::competition::is_disabled() && !pros::competition::is_autonomous()) {
      if (gameState != State::Driver) {
        setColor(leftWingLED);
        setColor(rightWingLED);
        setColor(frontLED);
        setColor(backLED);
        gameState = State::Driver;
        if (auton == 0) {
          driverTime = pros::millis();
        } else {
          driverTime = pros::millis() + 45000;
        }
      }

      if (time > 30000 && time < 30700 && auton != 0) {
        leftWingLED.set_all(0x00AA00);
        rightWingLED.set_all(0x00AA00);
        frontLED.set_all(0x00AA00);
        backLED.set_all(0x00AA00);
      }
      else if (time > 45000 && time < 45700) {
        leftWingLED.set_all(0xFFD800);
        rightWingLED.set_all(0xFFD800);
        frontLED.set_all(0xFFD800);
        backLED.set_all(0xFFD800);
      }
      else if (time > 45000 || auton != 0) {
        setColor(leftWingLED);
        setColor(rightWingLED);
        setColor(frontLED);
        setColor(backLED);
      }
    }
    
    pros::delay(10);
  }
}