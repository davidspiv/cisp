#include <iostream>
#include <string>

using String = std::string;

struct Month {
  String name;
  int inches;
};

void display(String output, bool carriageRtn = false);
int getInput();
int getInches(Month month);
std::string addCommas(int input);

int main() {
  const int numMonths = 12;
  const char* monthNameArr[numMonths] = {
      "january", "february", "march",     "april",   "may",      "june",
      "july",    "august",   "september", "october", "november", "december"};
  Month monthDataArr[numMonths];
  int total = 0, average, high, low;
  display("Enter inches of rainfall per month.", 1);

  for (int i = 0; i < numMonths; i++) {
    monthDataArr[i].name = monthNameArr[i];
    const int inches = getInches(monthDataArr[i]);
    monthDataArr[i].inches = inches;
    total += inches;
  }

  low, high = monthDataArr[0].inches;

  for (int i = 0; i < numMonths; i++) {
    const int inches = monthDataArr[i].inches;
    if (inches > high) high = inches;
    if (inches < high) low = inches;
  }

  display("\nTOTAL   : " + addCommas(total), 1);
  display("AVERAGE : " + addCommas(total / numMonths), 1);
  display("HIGH    : " + addCommas(high), 1);
  display("LOW     : " + addCommas(low), 1);
}

void display(String output, bool carriageRtn) {
  if (carriageRtn) output += "\n";

  std::cout << output;
}

int getInches(Month month) {
  display(month.name + ": ");
  return getInput();
}

int getInput() {
  int input;

  std::cin >> input;

  while (std::cin.fail()) {
    display("Input outside of integer range, try again", true);

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }
  return input;
}

std::string addCommas(int input) {
  String inputString = std::to_string(input);
  for (int i = inputString.size() - 3; i > 0; i = i - 3) {
    inputString.insert(i, ",");
  }

  return inputString;
}
