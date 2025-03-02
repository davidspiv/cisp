#include <iostream>

#include "ImageEditor.h"
#include "lodepng.h"
#include "picture.h"
using namespace std;
int main() {
  //   ImageEditor imageEditor("pikachu.png");
  //   imageEditor.test("test.PNG");

  ImageEditor imageEditor("pikachu.png");
  imageEditor.save("result.PNG");
}
