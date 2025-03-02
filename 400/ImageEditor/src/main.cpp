#include <iostream>

#include "ImageEditor.h"
#include "lodepng.h"
#include "picture.h"
using namespace std;
int main() {
  //   ImageEditor imageEditor("pikachu.png");
  //   imageEditor.test("test.PNG");

  //   ImageEditor imageEditor("pikachu.png");
  //   imageEditor.ascii("result.txt");

  //   ImageEditor imageEditor2("pikachu.png");
  //   imageEditor2.graysViaLightness();
  //   imageEditor2.save("percBright.PNG");

  ImageEditor imageEditor("pikachu.png");
  imageEditor.graysBasic();
  imageEditor.save("grays.png");
  //   imageEditor.ascii("result.txt");
}
