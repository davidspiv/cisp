#include "ImageEditor.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

ImageEditor::ImageEditor(const string& inFileName) : pic(Picture(inFileName)) {}

void ImageEditor::save(const string& outFileName) { pic.save(outFileName); }

double sRGBtoLin(double colorChannel) {
  // Send this function a decimal sRGB gamma encoded color value
  // between 0.0 and 1.0, and it returns a linearized value.

  if (colorChannel <= 0.04045) {
    return colorChannel / 12.92;
  } else {
    return pow(((colorChannel + 0.055) / 1.055), 2.4);
  }
}

double YtoLstar(double Y) {
  // Send this function a luminance value between 0.0 and 1.0,
  // and it returns L* which is "perceptual lightness"

  if (Y <= (216 / 24389.0)) {  // The CIE standard states 0.008856 but 216/24389
                               // is the intent for 0.008856451679036
    return Y * (24389 / 27.0);  // The CIE standard states 903.3, but 24389/27
                                // is the intent, making 903.296296296296296
  } else {
    return pow(Y, (1 / 3.0)) * 116 - 16;
  }
}

size_t scaleRange(double x) {
  const size_t currMax = 100;  // 765
  const size_t newMax = 69;

  return floor(newMax * x / currMax);
}

// from 0 to 100
double calcLightness(Color c) {
  const double r = c.r / 255.0;
  const double b = c.b / 255.0;
  const double g = c.g / 255.0;

  const double Y =
      0.2126 * sRGBtoLin(r) + 0.7152 * sRGBtoLin(g) + 0.0722 * sRGBtoLin(b);
  return YtoLstar(Y);
}

void ImageEditor::ascii(const string& outFileName) {
  const string asciiGrayscaleRange =
      "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
      "\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

  stringstream ss;
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    int pixSum = 0;
    int pixNum = 0;

    for (size_t i = 0; i < width; i++) {
      const int r = pic.red(i, j);
      const int g = pic.green(i, j);
      const int b = pic.blue(i, j);

      //   ss << calcLightness({r, g, b}) << endl;

      pixSum += calcLightness({r, g, b});  // r + g + b;
      pixNum++;

      if (!(j % 7) && !(i % 3)) {
        // ss << pixSum / pixNum << endl;
        ss << asciiGrayscaleRange[scaleRange(pixSum / pixNum)];
        pixSum = pixNum = 0;
      }
    }

    if (!(j % 7)) {
      ss << endl;
    }
  }

  ofstream out;
  out.open(outFileName);
  out << ss.str();
  out.close();
}

// (image negative)
ImageEditor& ImageEditor::operator-() {
  size_t width = pic.width();
  size_t height = pic.height();
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int red = 255 - pic.red(i, j);
      int green = 255 - pic.green(i, j);
      int blue = 255 - pic.blue(i, j);
      pic.set(i, j, red, green, blue);
    }
  }

  return *this;
};

// add c to all
ImageEditor& ImageEditor::operator+=(const Color& c) {
  size_t width = pic.width();
  size_t height = pic.height();
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int red = pic.red(i, j) + c.r;
      int green = pic.green(i, j) + c.g;
      int blue = pic.blue(i, j) + c.b;

      if (red < 0) {
        red = 0;
      } else if (red > 255) {
        red = 255;
      }

      if (green < 0) {
        green = 0;
      } else if (green > 255) {
        green = 255;
      }

      if (blue < 0) {
        blue = 0;
      } else if (blue > 255) {
        blue = 255;
      }

      pic.set(i, j, red, green, blue);
    }
  }

  return *this;
};

// subtract c from all pixels
ImageEditor& ImageEditor::operator-=(const Color& c) {
  return *this += {-c.r, -c.g, -c.b};
};

// compare to another image
bool ImageEditor::operator==(const ImageEditor& ie) const {
  size_t width = pic.width();
  size_t height = pic.height();
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      if (ie.pic.red(i, j) != pic.red(i, j)) return false;
      if (ie.pic.green(i, j) != pic.green(i, j)) return false;
      if (ie.pic.blue(i, j) != pic.blue(i, j)) return false;
    }
  }
  return true;
};

// compare to another image
bool ImageEditor::operator!=(const ImageEditor& ie) const {
  return !(*this == ie);
};

// expand by factor of n by n
ImageEditor& ImageEditor::operator*=(unsigned int n) {
  if (n < 1 || n > 10) throw runtime_error("N must be from 1 to 10");

  Picture picOut(pic.width() * n, pic.height() * n);

  size_t width = pic.width();
  size_t height = pic.height();
  int bigJ = 0;

  int squareSize = 12;

  for (size_t j = 0; j < height; j++) {
    int bigI = 0;

    for (size_t i = 0; i < width; i++) {
      int red;
      int green;
      int blue;
      if ((height % squareSize && !(i % squareSize)) ||
          (!(j % squareSize) && (i % squareSize))) {
        red = pic.red(i, j);
        green = pic.green(i, j);
        blue = pic.blue(i, j);
      } else {
        red = 255 - pic.red(i, j);
        green = 255 - pic.green(i, j);
        blue = 255 - pic.blue(i, j);
      }

      // create color cube
      for (size_t k = 0; k < n; k++) {
        for (size_t l = 0; l < n; l++) {
          picOut.set(bigI + k, bigJ + l, red, green, blue);
        }
      }

      bigI += n;
    }
    bigJ += n;
  }

  pic = picOut;

  return *this;
}
