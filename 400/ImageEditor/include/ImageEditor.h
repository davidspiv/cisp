#pragma once
#include <iostream>

#include "picture.h"
using namespace std;

struct Color {
  int r, g, b;
};

class ImageEditor {
public:
  explicit ImageEditor(const string &inFileName);
  void save(const string &outFileName);
  void asciiPrep();
  void ascii(const string &outFileName);
  void createTestImage(const string &outFileName);
  void grayscaleViaLuminance();
  void grayscaleViaLightness();
  void dither();
  void gaussianBlur(size_t kSize = 3);
  void sobelOperator();
  void bilinearResize(double factor);

  ImageEditor &
  operator-(); // Invert all colors and return *this (image negative)
  ImageEditor &operator-=(const Color &c);      // subtract c from all pixels
  ImageEditor &operator+=(const Color &c);      // add c to all pixels
  bool operator==(const ImageEditor &ie) const; // compare to another image
  bool operator!=(const ImageEditor &ie) const; // compare to another image
  ImageEditor &operator*=(unsigned int n);      // expand by factor of n by n

private:
  // void swapPixels(int x1, int y1, int x2, int y2);
  Picture pic;
};
