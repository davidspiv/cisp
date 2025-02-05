#include <cstdlib>

bool isPrime(unsigned long long n) {
  for (size_t i = 2; i < n - 1; i++) {
    if (!(n % i)) {
      return false;
    }
  }
  return true;
}

unsigned long long getPrime(unsigned long long min, unsigned long long max) {
  unsigned long long random;
  do {
    random = rand() % (max - min + 1) + min;
  } while (!isPrime(random));
  return random;
}

unsigned long long gcd(unsigned long long x, unsigned long long y) {
  unsigned long long remainder = x % y;
  while (remainder) {
    x = y;
    y = remainder;
    remainder = x % y;
  };

  return y;
}

unsigned long long lcm(unsigned long long x, unsigned long long y) {
  return (x * y) / gcd(x, y);
}

unsigned long long modInverse(unsigned long long e, unsigned long long lam) {
  for (size_t d = 1; d <= lam; d++) {
    bool test = d * e % lam == 1;
    if (test) {
      return d;
    }
  }
  return 0;
}

unsigned long long modExp(unsigned long long base, unsigned long long exp,
                          unsigned long long n) {
  unsigned long long ans = 1;
  for (unsigned long long i = 0; i < exp; i++) ans = (ans * base) % n;
  return ans;
}
