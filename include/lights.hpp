#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
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
  //ryan::Solenoid &Lwingus;
  //ryan::Solenoid &Rwingus;

public:
    int auton = 0;

    Lights(sylib::Addrled &underglowLED, sylib::Addrled &backLED, sylib::Addrled &leftDriveLED, sylib::Addrled &rightDriveLED)
        : underglowLED(underglowLED), backLED(backLED), leftDriveLED(leftDriveLED), rightDriveLED(rightDriveLED) 
        {};
        
    void init();
    void loop() override;
    void rotate();
    void setColor(sylib::Addrled &strip);
    void flash(sylib::Addrled &strip, int auton);
    void loadFile();
    unsigned char* readBMP(const char* filename);
    
};
} // namespace balls