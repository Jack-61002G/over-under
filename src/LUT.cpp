#include "LUT.h"
#include <vector>

int closestBinary(std::vector<double> list, double number) {
  if (list[list.size() - 1] <= number) {
    int nmber = list.size() - 1;
    return nmber;
  }
  if (list[0] >= number) {
    return 0;
  }
  int minimumI = 0;
  int maximumI = list.size() - 1;
  while (true) {
    int guess = std::floor((minimumI + maximumI) / 2);
    if (list[guess + 1] - list[guess] > number - list[guess] &&
        number - list[guess] >= 0) {
      return guess;
    } else if (number - list[guess] < 0) {
      maximumI = guess;
    } else {
      minimumI = guess;
    }
  }
}