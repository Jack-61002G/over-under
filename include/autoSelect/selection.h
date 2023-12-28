#pragma once

#include <string>

// selector configuration
#define HUE 300
#define DEFAULT 0
#define AUTONS "6 ball", "descore", "disrupt"

namespace selector {

extern int auton;
const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

} // namespace selector
