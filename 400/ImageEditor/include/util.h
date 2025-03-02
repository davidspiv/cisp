#include <cmath>

#include "ImageEditor.h"

// Input a decimal rbg gamma encoded color value between 0.0 and 1.0
// Returns a linearized value.
double rbgToLin(double channel);

// input luminance value between 0.0 and 1.0
// returns L* which is "perceptual lightness"
double YtoLstar(double Y);

size_t scaleRange(double x, size_t currMax, size_t newMax);

// return value from 0 to 100
double calcLightness(Color c);
