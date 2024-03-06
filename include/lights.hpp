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
  sylib::Addrled &leftDriveLED;
  sylib::Addrled &rightDriveLED;
  sylib::Addrled &backLED;
  sylib::Addrled &frontLED;

public:
    int auton = 0;

    Lights(sylib::Addrled &rightDriveLED, sylib::Addrled &leftDriveLED, sylib::Addrled &backLED, sylib::Addrled &frontLED)
          : leftDriveLED(leftDriveLED), rightDriveLED(rightDriveLED), backLED(backLED), frontLED(frontLED)
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