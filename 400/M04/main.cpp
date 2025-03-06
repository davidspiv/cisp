#include <iostream>
#include <string>

#include "Matrices.h"
#include "io.h"
int main() {
  Matrices::Matrix matrixB(1, 5);
  Matrices::Matrix matrixA(5, 1);

  // TEST

  matrixB(0, 0) = -5;
  matrixB(0, 1) = -8;
  matrixB(0, 2) = -10;
  matrixB(0, 3) = -8;
  matrixB(0, 4) = -5;

  matrixA(0, 0) = 5;
  matrixA(1, 0) = -4;
  matrixA(2, 0) = 0;
  matrixA(3, 0) = 4;
  matrixA(4, 0) = 5;

  std::cout << "a:" << std::endl;
  std::cout << matrixA << std::endl;
  std::cout << "b:" << std::endl;
  std::cout << matrixB << std::endl;
  std::cout << "c = a * b:" << std::endl;
  std::cout << matrixA * matrixB;
}
