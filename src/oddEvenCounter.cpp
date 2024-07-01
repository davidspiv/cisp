#include <time.h>

#include <iostream>
#include <string>

void display(std::string output, bool returnFlag = 0);

int main() {
  int even = 0, odd = 0;

  srand(time(0));  // random seed based on current time

  for (int i = 0; i < 100; i++) {
    if (rand() % 2) {
      even++;
      continue;
    }
    odd++;
  }

  display("Even : " + std::to_string(even), 1);
  display("Odd  : " + std::to_string(odd), 1);
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}
