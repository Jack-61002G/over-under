#include "robotconfig.h"
#include <atomic>
#include <cmath>
#include <iostream>


namespace balls {
class Lights : public ryan::TaskWrapper {

public:
    /*
    enum class TEAM { Red, Blue, Skills };
    enum class GAMESTATE { Initializing, Disabled, Autonomous, Driver };
    */

private:
    /*
    TEAM team;
    GAMESTATE gamestate;
    */

    static unsigned char* readBMP(char* filename);
    unsigned char *doinkerAnimData;

    int doinkerAnimTimestep = 0;
    sylib::Addrled &intakeLED;
    sylib::Addrled &doinkerLED;
    ryan::Solenoid &doinker;

public:
    Lights(sylib::Addrled &intakeLED, sylib::Addrled &doinkerLED, ryan::Solenoid &doinker)
    : intakeLED(intakeLED), doinkerLED(doinkerLED), doinker(doinker)
    {
        doinkerAnimData = readBMP("doinkerAnim.BMP");

        intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
        doinkerLED.clear();
    };
    
    // void update( TEAM team, GAMESTATE gamestate );

    void loop() override;

};
}