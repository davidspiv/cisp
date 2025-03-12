#include "ImageEditor.h"
using namespace std;

int main() {
  ImageEditor imageEditor("pikachu.png");
  //   imageEditor.grayscaleViaLightness();
  //   imageEditor.gaussianBlur(3);
  //   -imageEditor;
  //   imageEditor.sobelOperator();
  //   imageEditor.dither();
  //   imageEditor.asciiPrep();
  //   imageEditor *= 5;
  imageEditor.bilinearResize(1.5);
  imageEditor.save("result.png");
  //   imageEditor.ascii("result.txt");
}
