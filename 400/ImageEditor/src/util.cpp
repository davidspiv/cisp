#include <cmath>

#include "ImageEditor.h"

double rbgToLin(double channel) {
  channel /= 255.0;

  if (channel <= 0.04045) {
    return channel / 12.92;
  } else {
    return pow(((channel + 0.055) / 1.055), 2.4);
  }
}

double YtoLstar(double Y) {
  if (Y <= (216 / 24389.0)) {  // The CIE standard states 0.008856 but 216/24389
                               // is the intent for 0.008856451679036
    return Y * (24389 / 27.0);  // The CIE standard states 903.3, but 24389/27
                                // is the intent, making 903.296296296296296
  } else {
    return pow(Y, (1 / 3.0)) * 116 - 16;
  }
}

size_t scaleRange(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

// from 0 to 100
double calcLightness(Color c) {
  const double Y =
      0.2126 * rbgToLin(c.r) + 0.7152 * rbgToLin(c.g) + 0.0722 * rbgToLin(c.b);
  return YtoLstar(Y);
}
