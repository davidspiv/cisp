#include "ImageEditor.h"
#include "timer.h"
using namespace std;

int main() {
  Timer timer;
  ImageEditor imageEditor("lotus.png");
  //   imageEditor.grayscaleViaLightness();
  //   imageEditor.gaussianBlur(3);
  //   -imageEditor;
  //   imageEditor.sobelOperator();
  //   imageEditor.dither();
  //   imageEditor.asciiPrep();
  //   imageEditor *= 5;
  imageEditor.bilinearResize(0.0625);
  imageEditor.save("result.png");
  //   imageEditor.ascii("result.txt");
}
