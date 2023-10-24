#include "lights.hpp"
#include <fstream>
#include <string>

using namespace balls;

unsigned char* Lights::readBMP(const char* filename) {   
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f); 

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f); 
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            // flip the order of every 3 bytes
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    return data;
}

bool Lights::checkExists(const char* filename) {

    FILE* f;
    if ((f = fopen(filename, "r"))) return true;
    
    return false;
}

void Lights::loop() {
    intakeLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
    doinkerLED.set_all(sylib::Addrled::rgb_to_hex(200, 0, 0));
/**/
    unsigned char* doinkerAnimData = readBMP("/usd/doinkerAnim.BMP");
    
    while (true) {
        // find what frame in the animation we are
        if (doinker.getState()) {
            doinkerAnimTimestep++;
            if (doinkerAnimTimestep > 32) {doinkerAnimTimestep = 30;}
        } else {
            doinkerAnimTimestep--;
            if (doinkerAnimTimestep < 1) {doinkerAnimTimestep = 1;}
        }

            // retrieve the value of each pixel for that frame
            for(int i = 1; i<=32; i++) {
                doinkerLED.set_pixel(sylib::Addrled::rgb_to_hex(doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep)],  // R
                                                                    doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep) + 1], // G
                                                                    doinkerAnimData[3 * (i * 32 + doinkerAnimTimestep) + 2]),// B
                                                                    i ); // index of the pixel in the strip
            }

        pros::delay(10);
    }
}