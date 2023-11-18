#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "main.h"
#include <atomic>
#include <cmath>
#include <iostream>

namespace balls {

class Lights : public ryan::TaskWrapper {

private:

  enum class State { Disabled, Enabled, Null};
  State gameState = State::Null;
  int doinkerAnimTimestep;
  unsigned char *doinkerAnimData;
  sylib::Addrled &underglowLED;
  sylib::Addrled &intakeLED;
  sylib::Addrled &doinkerLED;
  sylib::Addrled &rightDriveLED;
  sylib::Addrled &leftDriveLED;
  ryan::Solenoid &doinker;

public:
    Lights(sylib::Addrled &underglowLED, sylib::Addrled &doinkerLED, sylib::Addrled &intakeLED, sylib::Addrled &leftDriveLED, sylib::Addrled &rightDriveLED, ryan::Solenoid &doinker)
        : underglowLED(underglowLED), intakeLED(intakeLED), doinkerLED(doinkerLED),leftDriveLED(leftDriveLED), rightDriveLED(rightDriveLED), doinker(doinker) 
        {};
        
    void init();
    void loop() override;
    void rotate();
    void setColor(int auton);
    void loadFile();
    unsigned char* readBMP(const char* filename);
    
};
} // namespace balls