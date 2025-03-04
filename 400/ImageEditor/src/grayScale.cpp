#include "../include/ImageEditor.h"
#include "../include/util.h"
#include <cmath>

void ImageEditor::grayscaleViaLuminance() { pic = pic.grays(); }


// convert sRGB channel to a linear value
double linearizeRGB(int channel) {
  double normalizedRGB = channel / 255.0;

  if (normalizedRGB <= 0.04045) {
    return normalizedRGB / 12.92;
  } else {
    return pow(((normalizedRGB + 0.055) / 1.055), 2.4);
  }
}


// estimates perceived lightness (L*) in the CIELAB colorspace. Returns a
// value from 0 to 100 inclusive
double calcLightness(Color c) {
  // apply standard coefficients for sRGB to get luminance (Y)
  const double linearR = 0.2126 * linearizeRGB(c.r);
  const double linearG = 0.7152 * linearizeRGB(c.g);
  const double linearB = 0.0722 * linearizeRGB(c.b);
  const double Y = linearR + linearG + linearB;

  // convert Y to perceived lightness (L*)
  if (Y <= (216 / 24389.0)) {  // more exact than min CIE standard of 0.008856
    return Y * (24389 / 27.0); // more exact than min CIE standard of 903.3
  } else {
    return cbrt(Y) * 116 - 16;
  }
}


// converts sRGB to L* in the CIELAB colorspace. Then is scales L* back up to a
// grayscale sRGB channel value. Modifies picture object
void ImageEditor::grayscaleViaLightness() {
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      Color c = {pic.red(i, j), pic.green(i, j), pic.blue(i, j)};
      const double lightness = calcLightness(c);
      // is it correct to linearly scale lightness up to RGB range??
      const int grayComponent = scaleValue(lightness, 100, 255);
      pic.set(i, j, grayComponent, grayComponent, grayComponent);
    }
  }
};
