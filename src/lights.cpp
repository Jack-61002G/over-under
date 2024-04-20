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
    strip.set_all(0x700000);

  } else if (selector::auton < 0) {
    strip.set_all(0x000070);

  } else {
    strip.set_all(0x600060);
  }
}



void Lights::loop() {

  enum class State { Start, Disabled, Enabled };
  State gameState = State::Start;

  int timeEnabled;

  bool timeWarning;

  int intakeBright;
  bool intakeDirecion;

  bool LwingState;
  int LwingTime;

  bool RwingState;
  int RwingTime;

  while (true) {

    // disabled state
    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      gameState = State::Disabled;

      // rainbow lights
      if (selector::auton != 0 && timeEnabled > 0 && pros::millis() - timeEnabled > 10000) {
        frontLED.gradient(0xFF0000, 0x0000FF);
        frontLED.cycle(*frontLED, 5);
        backLED.gradient(0xFF0000, 0x0000FF);
        backLED.cycle(*backLED, 5);
        leftWingLED.gradient(0xFF0000, 0x0000FF);
        leftWingLED.cycle(*leftWingLED, 5, 0, true);
        rightWingLED.gradient(0xFF0000, 0x0000FF);
        rightWingLED.cycle(*rightWingLED, 5);
      }

      // rotating lights
      else {
        frontLED.set_all(0x600060);
        backLED.set_all(0x600060);
        leftWingLED.clear();
        rightWingLED.clear();

        leftWingLED.gradient(0x000000, 0xDD00DD, 0, 3);
        leftWingLED.cycle(*leftWingLED, 5);
        rightWingLED.gradient(0x000000, 0xDD00DD, 0, 3);
        rightWingLED.cycle(*rightWingLED, 5);
      }
    }


    // enabled state
    if (!pros::competition::is_disabled()) {

      // first time entering enabled
      if (gameState != State::Enabled) {
        gameState = State::Enabled;

        setColor(frontLED);
        setColor(backLED);
        setColor(leftWingLED);
        setColor(rightWingLED);

        timeEnabled = pros::millis();
      }


      // color changing for time warnings
      if (75000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 75700 && selector::auton != 0) {
        timeWarning = true;
        frontLED.set_all(0x009900);
        backLED.set_all(0x009900);
        leftWingLED.set_all(0x009900);
        rightWingLED.set_all(0x009900);
      
      } else if (90000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 90700 && selector::auton != 0) {
        timeWarning = true;
        frontLED.set_all(0xF0D000);
        backLED.set_all(0xF0D000);
        leftWingLED.set_all(0xF0D000);
        rightWingLED.set_all(0xF0D000);
      
      } else if (timeWarning) {
        timeWarning = false;
        setColor(frontLED);
        setColor(backLED);
        setColor(leftWingLED);
        setColor(rightWingLED);
      }


      // intake animation
      if (intake.getState() == Intake::STATE::IDLE && !timeWarning) {
        setColor(frontLED);
        intakeBright = 130;
        intakeDirecion = false;
      }
      if (intake.getState() != Intake::STATE::IDLE && !timeWarning) {
        if (intakeDirecion) {intakeBright+=25;}
        else {intakeBright-=25;}
        if (intakeBright > 120) {intakeDirecion = false;}
        if (intakeBright < 25) {intakeDirecion = true;}

        if (selector::auton > 0) {
          frontLED.set_all(sylib::Addrled::rgb_to_hex(intakeBright, 0, 0));
        }
        else if (selector::auton < 0) {
          frontLED.set_all(sylib::Addrled::rgb_to_hex(0, 0, intakeBright));
        }
        else {
          frontLED.set_all(sylib::Addrled::rgb_to_hex(intakeBright * 0.75, 0, intakeBright * 0.75));
        }
      }


      // wing animation
      if (Lwingus.getState() && !LwingState && !timeWarning) {
        LwingState = true;
        LwingTime = pros::millis();
        if (selector::auton > 0) {
          leftWingLED.set_all(0xFF6020);
        }
        else if (selector::auton < 0) {
          leftWingLED.set_all(0x4040FF);
        }
        else {
          leftWingLED.set_all(0xBB40BB);
        }
      } if (LwingState && pros::millis() - LwingTime > 180 && !timeWarning) {
        setColor(leftWingLED);
      } if (!Lwingus.getState() && LwingState) {
        LwingState = false;
      }

      if (Rwingus.getState() && !RwingState && !timeWarning) {
        RwingState = true;
        RwingTime = pros::millis();
        if (selector::auton > 0) {
          rightWingLED.set_all(0xFF6020);
        }
        else if (selector::auton < 0) {
          rightWingLED.set_all(0x4040FF);
        }
        else {
          rightWingLED.set_all(0xC040C0);
        }
      } if (RwingState && pros::millis() - RwingTime > 180 && !timeWarning) {
        setColor(rightWingLED);
      } if (!Rwingus.getState() && RwingState) {
        RwingState = false;
      }
    }

  pros::delay(30);
  }
}