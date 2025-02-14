#include <climits>
#include <cstdlib>
#include <iostream>

#include "RSA.h"
#include "io.h"

int main() {
  unsigned long long p, q, n, lambda, d, e, m, c;
  unsigned int seed;

  print("Enter a seed: ");
  std::cin >> seed;
  srand(seed);
  p = getPrime(UCHAR_MAX, USHRT_MAX);
  q = getPrime(UCHAR_MAX, USHRT_MAX);
  print(p, "p");
  print(q, "q");
  n = p * q;
  print(n, "n");
  lambda = lcm(p - 1, q - 1);
  print(lambda, "lambda");
  e = getPrime(2, lambda - 1);
  print(e, "e");
  d = modInverse(e, lambda);
  print(d, "d");
  print("n = " + std::to_string(n) + " " + "e = " + std::to_string(e),
        "Public key");
  print("n = " + std::to_string(n) + " " + "d = " + std::to_string(d),
        "Private key");
  print("Enter a positive number less than " + std::to_string(n) + ":");
  std::cin >> m;
  c = modExp(m, e, n);
  print(c, "Cipher");
  c = modExp(c, d, n);
  print(c, "Decrypted cipher");
}
