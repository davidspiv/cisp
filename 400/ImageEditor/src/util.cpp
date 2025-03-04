// https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

#include <algorithm>
#include <cmath>

#include "ImageEditor.h"

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

size_t clamp(double x, size_t max) {
  if (x > max)
    return max;
  return x;
}


int mirrorPixel(int x, int max) {
  if (x < 0)
    return -x; // Mirror left/top side
  if (x >= max)
    return 2 * (max - 1) - x; // Mirror right/bottom side
  return x;
}
