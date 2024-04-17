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

  int timeEnabled; // time in msec
  int timewarning; // 0, 1, 2
  bool intakeAnim;
  bool LwingState;
  bool RwingState;

  while (true) {

    // disabled state
    if (pros::competition::is_disabled() && gameState != State::Disabled) {
      gameState = State::Disabled;

      frontLED.set_all(0x600060);
      backLED.set_all(0x600060);
      leftWingLED.clear();
      rightWingLED.clear();

      leftWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
      leftWingLED.cycle(*leftWingLED, 5);
      rightWingLED.gradient(0x000000, 0xFF00FF, 0, 3);
      rightWingLED.cycle(*rightWingLED, 5);
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
      if (75000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 75700) {
        timewarning = 1;
        frontLED.set_all(0x00AA00);
        backLED.set_all(0x00AA00);
        leftWingLED.set_all(0x00AA00);
        rightWingLED.set_all(0x00AA00);
      
      } else if (90000 < pros::millis() - timeEnabled && pros::millis() - timeEnabled < 90700) {
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


      // starting intake animation
      if (intake.getState() != Intake::STATE::IDLE && !intakeAnim && timewarning == 0) {
        intakeAnim = true;

        if (selector::auton > 0) {
          frontLED.gradient(0x600000, 0xFF0000);
          frontLED.cycle(*frontLED, 10);
        }  

        else if (selector::auton < 0) {
          frontLED.gradient(0x000060, 0x0000FF);
          frontLED.cycle(*frontLED, 10);
        }

        else {
          frontLED.gradient(0x400040, 0x990099);
          frontLED.cycle(*frontLED, 10);
        }
      }
      // stopping intake animation
      if (intake.getState() == Intake::STATE::IDLE && intakeAnim && timewarning == 0) {
        intakeAnim = false;
        setColor(frontLED);
      }


      // wing animation
      //if (Lwingus.getState() && !LwingState) {
      //  LwingState = true;
      //  leftWingLED.set_all(0x00FF00);
      //}
    }

  pros::delay(50);
  }
}