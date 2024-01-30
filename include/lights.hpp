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

  enum class State { Off, Disabled, Enabled, Green, Yellow, Rainbow};
  State lightState;
  int driverStartTime;
  sylib::Addrled &underglowLED;
  sylib::Addrled &backLED;
  sylib::Addrled &rightDriveLED;
  sylib::Addrled &leftDriveLED;

public:
    int auton = 0;

    Lights(sylib::Addrled &underglowLED, sylib::Addrled &backLED, sylib::Addrled &leftDriveLED, sylib::Addrled &rightDriveLED)
          : underglowLED(underglowLED), backLED(backLED), leftDriveLED(leftDriveLED), rightDriveLED(rightDriveLED)
          {};
        
    void loop() override;

    void OffTick();

    void DisabledStart(); void DisabledTick();
    
    void EnabledStart(); void EnabledTick();

    void GreenStart(); void GreenTick();

    void YellowStart(); void YellowTick();

    void RainbowStart(); void RainbowTick();
    
};
} // namespace balls