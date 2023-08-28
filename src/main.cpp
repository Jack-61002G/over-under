#include "main.h"
#include "gif-pros/gifclass.hpp"
#include "lemlib/asset.hpp"
#include "robotconfig.h"

ASSET(h_gif)
ASSET(skillsmatchload_txt)

void initialize() {}

void autonomous() { chassis.follow(skillsmatchload_txt, 5000, 5); }

void disabled() {

  intakeLED.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
  intakeLED.cycle(*intakeLED, 10);

  // Store the time at the start of the loop
  std::uint32_t clock = sylib::millis();
  while (true) {

    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

void opcontrol() {
  std::uint32_t clock = sylib::millis();
  Gif gif(h_gif, lv_scr_act());

  while (true) {
    sylib::delay_until(&clock, 10);
  }
}
