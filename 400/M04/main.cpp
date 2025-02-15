#include <iostream>
#include <string>

#include "Matrices.h"
#include "io.h"
int main() {
  Matrices::Matrix matrixA(2, 2);
  Matrices::Matrix matrixB(2, 4);

  // TEST

  matrixA(0, 0) = 0;
  matrixA(0, 1) = -1;
  matrixA(1, 0) = 1;
  matrixA(1, 1) = 0;

  matrixB(0, 0) = 1;
  matrixB(0, 1) = 0.866025;
  matrixB(0, 2) = 1;
  matrixB(0, 3) = 0.5;
  matrixB(1, 0) = 0;
  matrixB(1, 1) = 0.5;
  matrixB(1, 2) = 1;
  matrixB(1, 3) = 0.866025;

  std::cout << "a:" << std::endl;
  std::cout << matrixA << std::endl;
  std::cout << "b:" << std::endl;
  std::cout << matrixB << std::endl;
  std::cout << "c = b + b:" << std::endl;
  std::cout << matrixB + matrixB << std::endl;
  std::cout << "c = a * b:" << std::endl;
  std::cout << matrixA * matrixB;
}
