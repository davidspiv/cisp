#include "../include/ImageEditor.h"
#include "../include/util.h"
#include <fstream>
#include <iostream>
#include <sstream>

// void ImageEditor::asciiPrep() {
//   size_t width = pic.width();
//   size_t height = pic.height();
//   Picture newPic(width / 3, height / 6, 0, 0, 0);
//   int rSum = 0;
//   int gSum = 0;
//   int bSum = 0;
//   int count = 0;
//   for (size_t j = 0; j < height; j++) {
//     for (size_t i = 0; i < width; i++) {
//       rSum += pic.red(i, j);
//       gSum += pic.green(i, j);
//       bSum += pic.blue(i, j);
//       count++;
//       if (!(j % 6) && !(i % 3)) {
//         const uint8_t r = rSum / count;
//         const uint8_t g = gSum / count;
//         const uint8_t b = bSum / count;

//         newPic.set(i / 3, j / 6, r, g, b);
//         rSum = gSum = bSum = count = 0;
//       }
//     }
//     rSum = gSum = bSum = count = 0;
//   }
//   pic = newPic;
// }

void ImageEditor::asciiPrep() {
  size_t width = pic.width();
  size_t height = pic.height();
  Picture newPic(width / 3, height / 6, 0, 0, 0);

  for (size_t j = 0; j < height; j += 6) {
    for (size_t i = 0; i < width; i += 3) {
      int rSum = 0, gSum = 0, bSum = 0;
      int count = 0;

      for (size_t y = 0; y < 6 && (j + y) < height; ++y) {
        for (size_t x = 0; x < 3 && (i + x) < width; ++x) {
          rSum += pic.red(i + x, j + y);
          gSum += pic.green(i + x, j + y);
          bSum += pic.blue(i + x, j + y);
          count++;
        }
      }

      if (count > 0) {
        uint8_t r = rSum / count;
        uint8_t g = gSum / count;
        uint8_t b = bSum / count;
        newPic.set(i / 3, j / 6, r, g, b);
      }
    }
  }
  pic = newPic;
}


void ImageEditor::ascii(const string &outFileName) {
  const string asciiSorted =
      R"(@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iF|(7J)vTLs?z/*cr!+<>;=^,_:'-.` )";

  stringstream ss;
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {

      if (pic.red(i, j) != pic.green(i, j) || pic.red(i, j) != pic.blue(i, j)) {
        throw invalid_argument("non-grayscale pixel detected");
      }

      ss << asciiSorted[scaleValue(pic.red(i, j), 255,
                                   asciiSorted.length() - 1)];
    }
    ss << endl;
  }

  ofstream out;
  out.open(outFileName);
  out << ss.str();
  out.close();
}
