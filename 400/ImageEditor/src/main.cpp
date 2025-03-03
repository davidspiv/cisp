#include <iostream>

#include "ImageEditor.h"
#include "lodepng.h"
#include "picture.h"
using namespace std;
int main() {
  ImageEditor imageEditor("pikachu.png");
  imageEditor.graysViaLightness();
  //   imageEditor.gaussianBlur();
  imageEditor.sobelFilter();
  imageEditor.save("result.png");
  //   imageEditor.ascii("result.txt");
}
