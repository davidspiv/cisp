#include "../include/ImageEditor.h"
#include "../include/timer.h"

#include <string>

using namespace std;

ImageEditor::ImageEditor(const string &inFileName) : pic(Picture(inFileName)) {}

void ImageEditor::save(const string &outFileName) { pic.save(outFileName); }

// (image negative)
ImageEditor &ImageEditor::operator-() {
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
ImageEditor &ImageEditor::operator+=(const Color &c) {
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
ImageEditor &ImageEditor::operator-=(const Color &c) {
  return *this += {-c.r, -c.g, -c.b};
};

// compare to another image
bool ImageEditor::operator==(const ImageEditor &ie) const {
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      if (ie.pic.red(i, j) != pic.red(i, j))
        return false;
      if (ie.pic.green(i, j) != pic.green(i, j))
        return false;
      if (ie.pic.blue(i, j) != pic.blue(i, j))
        return false;
    }
  }
  return true;
};

// compare to another image
bool ImageEditor::operator!=(const ImageEditor &ie) const {
  return !(*this == ie);
};

// expand by factor of n by n
ImageEditor &ImageEditor::operator*=(unsigned int n) {
  Timer timer;

  if (n < 1 || n > 10)
    throw runtime_error("N must be from 1 to 10");

  //   pic.Scale(n);

  size_t width = pic.width();
  size_t height = pic.height();

  Picture picOut(width * n, height * n, 20, 20, 20);

  for (size_t j = 0; j < width; j++) {
    for (size_t i = 0; i < height; i++) {
      int r = pic.red(i, j);
      int g = pic.green(i, j);
      int b = pic.blue(i, j);

      for (size_t k = 0; k < n; k++) {
        for (size_t l = 0; l < n; l++) {
          picOut.set(i * n + k, j * n + l, r, g, b);
        }
      }
    }
  }

  for (size_t j = 0; j < width * n; j++) {
    for (size_t i = 0; i < height * n; i++) {


      picOut.set(i, j, 10, 10, 10);
    }
  }

  //   pic = picOut;
  return *this;
}

void ImageEditor::createTestImage(const string &outFileName) {
  size_t width = pic.width();
  size_t height = pic.height();
  bool isBlack = 1;

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      if (isBlack) {
        pic.set(i, j, 230, 230, 230);
      } else {
        pic.set(i, j, 20, 20, 20);
      }
      if (!(i % 10)) {
        isBlack = !isBlack;
      }
    }
  }

  pic.save("testImage.PNG");
};
