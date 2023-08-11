#include "trapevel.h"
#include "main.h"

trapevel::trapevel(double UP, double DOWN, double VEL, double kp) {
  this->k_UP = UP;
  this->k_DOWN = DOWN;
  this->k_VEL = VEL;
  this->kP = kp;
}

double trapevel::update(double velocity, double actual) {
  double acceleration = (velocity - prevVelocity) / 0.02;
  prevVelocity = velocity;
  if (acceleration > 0) {
    return k_UP * acceleration + k_VEL * velocity + kP * (velocity - actual);
  } else {
    return k_DOWN * acceleration + k_VEL * velocity + kP * (velocity - actual);
  }
}

void trapevel::reset() { prevVelocity = 0; }