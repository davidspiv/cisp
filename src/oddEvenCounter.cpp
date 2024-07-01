#include <time.h>

#include <iostream>
#include <string>

using String = std::string;

void display(String output, bool returnFlag = 0);

int main() {
  int even = 0, odd = 0;

  srand(time(0));

  for (int i = 0; i < 100; i++) {
    if (!(rand() % 2)) {
      even++;
    } else {
      odd++;
    };
  }

  display("Even : " + std::to_string(even), 1);
  display("Odd  : " + std::to_string(odd), 1);
}

void display(String output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}
