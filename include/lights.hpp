#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "intake.h"
#include "main.h"
#include <atomic>
#include <cmath>
#include <iostream>

#pragma once

namespace balls {

class Lights : public ryan::TaskWrapper {

private:

  enum class State { Start, Disabled, Driver, Auto, Custom };
  State gameState = State::Disabled;
  sylib::Addrled &leftWingLED;
  sylib::Addrled &rightWingLED;
  sylib::Addrled &backLED;
  sylib::Addrled &frontLED;



public:

    Lights(sylib::Addrled &rightWingLED, sylib::Addrled &leftWingLED, sylib::Addrled &backLED, sylib::Addrled &frontLED)
          : leftWingLED(leftWingLED), rightWingLED(rightWingLED), backLED(backLED), frontLED(frontLED)
          {};
        
    void init();
    void loop() override;
    static void setColor(sylib::Addrled &strip);
    
};
} // namespace balls