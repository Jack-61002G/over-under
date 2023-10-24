#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "main.h"
#include <atomic>
#include <cmath>
#include <iostream>

namespace balls {

class Lights : public ryan::TaskWrapper {

private:

  int doinkerAnimTimestep = 1;
  sylib::Addrled &intakeLED;
  sylib::Addrled &doinkerLED;
  ryan::Solenoid &doinker;

public:
    Lights(sylib::Addrled &intakeLED, sylib::Addrled &doinkerLED, ryan::Solenoid &doinker)
        : intakeLED(intakeLED), doinkerLED(doinkerLED), doinker(doinker) 
        {};

    void loop() override;
    unsigned char* readBMP(const char* filename);
    bool checkExists(const char* filename);
};
} // namespace balls