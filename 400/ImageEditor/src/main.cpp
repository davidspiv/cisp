#include <iostream>

#include "ImageEditor.h"
#include "lodepng.h"
#include "picture.h"
using namespace std;
int main() {
  ImageEditor imageEditor("pikachu.png");
  //   imageEditor.grayscaleViaLightness();
  //   imageEditor.gaussianBlur(21);
  imageEditor.sobelFilter(3);
  //   -imageEditor;
  imageEditor.save("result.png");
  //   imageEditor.ascii("result.txt");
}
