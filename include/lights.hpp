#include "Solenoid.hpp"
#include "TaskWrapper.hpp"
#include "sylib/sylib.hpp"
#include <atomic>
#include <cmath>
#include <iostream>

namespace balls {
class Lights : public ryan::TaskWrapper {
  /*
  enum class TEAM { Red, Blue, Skills };
  enum class GAMESTATE { Initializing, Disabled, Autonomous, Driver };
  */

private:
  /*
  TEAM team;
  GAMESTATE gamestate;
  */

  //static unsigned char *readBMP();
  //unsigned char *doinkerAnimData;

  int doinkerAnimTimestep = 0;
  sylib::Addrled &intakeLED;
  sylib::Addrled &doinkerLED;
  ryan::Solenoid &doinker;

public:
  Lights(sylib::Addrled &intakeLED, sylib::Addrled &doinkerLED, ryan::Solenoid &doinker)
        : intakeLED(intakeLED), doinkerLED(doinkerLED), doinker(doinker) 
        {
    //doinkerAnimData = readBMP();
    intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
    doinkerLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
        };

  // void update( TEAM team, GAMESTATE gamestate );

  void loop() override;
};
} // namespace balls