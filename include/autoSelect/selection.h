#pragma once

#include <string>

//selector configuration
#define HUE 300
#define DEFAULT 1
#define AUTONS "Close mid", "close", "Far mid", "Far"

namespace selector{

extern int auton;
const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

}
