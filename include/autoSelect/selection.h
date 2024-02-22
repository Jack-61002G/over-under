#pragma once

#include <string>

// selector configuration
#define HUE 72
#define DEFAULT 0
#define AUTONS "rush", "disrupt", "swap", "descore"

namespace selector {

extern int auton;
const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

} // namespace selector
