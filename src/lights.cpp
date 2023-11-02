#include "lights.hpp"
#include "autoSelect/selection.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace balls;

unsigned char *Lights::readBMP(const char *filename) {
  int i;
  FILE *f = fopen(filename, "rb");
  unsigned char info[54];

  // read the 54-byte header
  fread(info, sizeof(unsigned char), 54, f);

  // extract image height and width from header
  int width = *(int *)&info[18];
  int height = *(int *)&info[22];

  // allocate 3 bytes per pixel
  int size = 3 * width * height;
  unsigned char *data = new unsigned char[size];

  // read the rest of the data at once
  fread(data, sizeof(unsigned char), size, f);
  fclose(f);

  return data;
}

bool Lights::checkExists(const char *filename) {

  FILE *f;
  if ((f = fopen(filename, "r")))
    return true;

  return false;
}

void Lights::loop() {
  unsigned char *doinkerAnimData;
  bool loop = true;

  intakeLED.gradient(0x750000, 0x750005, 0, 0, false, true);
  intakeLED.cycle(*intakeLED, 5);
  underglowLED.gradient(0x750000, 0x750005, 0, 0, false, true);
  underglowLED.cycle(*underglowLED, 5);
  doinkerLED.gradient(0x750000, 0x750005, 0, 0, false, true);
  doinkerLED.cycle(*doinkerLED, 5);

  while(!pros::competition::is_disabled()) {pros::delay(100);}
  while(pros::competition::is_disabled()) {pros::delay(100);}

  if (selector::auton > 0) {
    intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
    underglowLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
    doinkerAnimData = readBMP("/usd/doinkerAnim.BMP");
  } else if (selector::auton < 0) {
    intakeLED.set_all(sylib::Addrled::rgb_to_hex(0, 0, 200));
    underglowLED.set_all(sylib::Addrled::rgb_to_hex(0, 0, 200));
    doinkerAnimData = readBMP("/usd/doinkerAnim2.BMP");
  } else {
    pauseTask();
    loop = false;
  }

  doinkerAnimTimestep = 32;

  while (true) {
    if (loop) {

      // find what frame in the animation we are
      if (!doinker.getState()) {
        doinkerAnimTimestep++;
        if (doinkerAnimTimestep > 32) {
          doinkerAnimTimestep = 32;
        }
      } else {
        doinkerAnimTimestep--;
        if (doinkerAnimTimestep < 1) {
          doinkerAnimTimestep = 3;
        }
      }

      // retrieve the value of each pixel for that frame
      for (int i = 0; i < 32; i++) {
        doinkerLED.set_pixel(
            sylib::Addrled::rgb_to_hex(
                doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep) - 1],  // B
                doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep) - 2],  // G
                doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep) - 3]), // R
            i); // index of the pixel in the strip
      }
    }
    pros::delay(18);
  }
}