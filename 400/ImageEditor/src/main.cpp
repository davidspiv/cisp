#include "ImageEditor.h"
using namespace std;

int main() {
  ImageEditor imageEditor("sea.png");
  imageEditor.grayscaleViaLightness();
  //   imageEditor.gaussianBlur(5);
  //   imageEditor.sobelOperator();
  //   -imageEditor;
  //   imageEditor.asciiPrep();
//   imageEditor.dither();
  imageEditor.save("result.png");
  //   imageEditor.ascii("result.txt");
}
