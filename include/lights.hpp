#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "sylib/sylib.hpp"
#include <atomic>
#include <cmath>
#include <iostream>

namespace balls {

class Lights : public ryan::TaskWrapper {

private:

  int doinkerAnimTimestep = 0;
  sylib::Addrled &intakeLED;
  sylib::Addrled &doinkerLED;
  ryan::Solenoid &doinker;

public:
    Lights(sylib::Addrled &intakeLED, sylib::Addrled &doinkerLED, ryan::Solenoid &doinker)
        : intakeLED(intakeLED), doinkerLED(doinkerLED), doinker(doinker) 
        {
            intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
            doinkerLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
        };

    void loop() override;
    unsigned char* readBMP(const char* filename);
};
} // namespace balls