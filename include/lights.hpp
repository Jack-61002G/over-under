#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "intake.h"
#include "main.h"
#include <atomic>
#include <cmath>
#include <iostream>

namespace balls {

class Lights : public ryan::TaskWrapper {

private:

  enum class State { Disabled, Driver, Skills};
  State gameState = State::Disabled;
  int doinkerAnimTimestep;
  unsigned char *doinkerAnimData;
  sylib::Addrled &underglowLED;
  sylib::Addrled &backLED;
  sylib::Addrled &rightDriveLED;
  sylib::Addrled &leftDriveLED;
  ryan::Solenoid &Lwingus;
  ryan::Solenoid &Rwingus;
  Intake &intake;

public:
    int auton = 0;

    Lights(sylib::Addrled &underglowLED, sylib::Addrled &backLED, sylib::Addrled &leftDriveLED, sylib::Addrled &rightDriveLED,
          ryan::Solenoid &Lwingus, ryan::Solenoid &Rwingus, Intake &intake)
          : underglowLED(underglowLED), backLED(backLED), leftDriveLED(leftDriveLED), rightDriveLED(rightDriveLED),
          Lwingus(Lwingus), Rwingus(Rwingus), intake(intake)
          {};
        
    void init();
    void loop() override;
    void rotate();
    void flow(sylib::Addrled &strip);
    void setColor(sylib::Addrled &strip);
    void flash(sylib::Addrled &strip);
    void loadFile();
    unsigned char* readBMP(const char* filename);
    
};
} // namespace balls