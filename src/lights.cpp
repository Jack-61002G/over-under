#include "lights.hpp"
#include "autoSelect/selection.h"
#include "display/lv_objx/lv_gauge.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace balls;


// Off State
void Lights::OffTick() {
  if (pros::competition::is_disabled()) { DisabledStart(); }
}


// Disabled State
void Lights::DisabledStart() {
  lightState = State::Disabled;

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
void Lights::DisabledTick() {
  if (!pros::competition::is_disabled())  { EnabledStart(); driverStartTime = pros::millis();}
}


// Enabled State
void Lights::EnabledStart() {
  lightState = State::Enabled;
  
  if (selector::auton > 0) {
    underglowLED.set_all(0x990000);
    backLED.set_all(0x990000);
    leftDriveLED.set_all(0x990000);
    rightDriveLED.set_all(0x990000);
  } 
  else if (selector::auton < 0) {
    underglowLED.set_all(0x000099);
    backLED.set_all(0x000099);
    leftDriveLED.set_all(0x000099);
    rightDriveLED.set_all(0x000099);
  } 
  else {
    underglowLED.set_all(0x600060);
    backLED.set_all(0x600060);
    leftDriveLED.set_all(0x600060);
    rightDriveLED.set_all(0x600060);
  }
}
void Lights::EnabledTick() {
  if (pros::competition::is_disabled()) {DisabledStart(); return;}
  if (pros::competition::is_autonomous()) {EnabledStart(); driverStartTime = 0; return;}
  if (driverStartTime < 1) {return;}
  int time = pros::millis() - driverStartTime;
  if (time > 75000 && time < 75600) {GreenStart(); return;}
  if (time > 90000 && time < 90600) {YellowStart(); return;}
  if (time > 10000) {RainbowStart(); return;}
}


// Green State
void Lights::GreenStart() {
  lightState = State::Green;

  underglowLED.set_all(0x00FF00);
  backLED.set_all(0x00FF00);
  leftDriveLED.set_all(0x00FF00);
  rightDriveLED.set_all(0x00FF00);
}
void Lights::GreenTick() {
  if (pros::competition::is_disabled()) {DisabledStart(); return;}
  if (pros::competition::is_autonomous()) {EnabledStart(); driverStartTime = 0; return;}
  if (pros::millis() - driverStartTime > 75600) {EnabledStart(); return;}
}


// Yellow State
void Lights::YellowStart() {
  lightState = State::Yellow;

  underglowLED.set_all(0xAA8800);
  backLED.set_all(0xAA8800);
  leftDriveLED.set_all(0xAA8800);
  rightDriveLED.set_all(0xAA8800);
}
void Lights::YellowTick() {
  if (pros::competition::is_disabled()) {DisabledStart(); return;}
  if (pros::competition:: is_autonomous()) {EnabledStart(); driverStartTime = 0; return;}
  if (pros::millis() - driverStartTime > 90600) {EnabledStart(); return;}
}


//Rainbow State
void Lights::RainbowStart() {
  lightState = State::Rainbow;

  underglowLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
  underglowLED.cycle(*underglowLED, 5);
  backLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
  backLED.cycle(*backLED, 5);
  leftDriveLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
  leftDriveLED.cycle(*leftDriveLED, 5);
  rightDriveLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
  rightDriveLED.cycle(*rightDriveLED, 5);
}
void Lights::RainbowTick() {
  if (pros::competition::is_disabled()) {driverStartTime = 0;}
  if (!pros::competition::is_disabled() && driverStartTime == 0) {EnabledStart(); driverStartTime = pros::millis(); return;}
}



void Lights::loop() {

  enum class State { Off, Disabled, Enabled, Green, Yellow, Rainbow};
  State lightState = State::Off;

  int driverStartTime;

  while (true) {
    
    switch(lightState) {

    case State::Off: {
      OffTick();
    }
    case State::Disabled: {
      DisabledTick();
    }
    case State::Enabled: {
      EnabledTick();
    }
    case State::Green: {
      GreenTick();
    }
    case State::Yellow: {
      YellowTick();
    }
    case State::Rainbow: {
      RainbowTick();
    }}
}}