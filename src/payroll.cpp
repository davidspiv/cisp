#include <math.h>

#include <iostream>
#include <string>

using String = std::string;

void display(String output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

bool isValidPay(float input) { return input >= 7.50 && input <= 18.25; }
bool isValidHours(int hours) { return hours >= 0 && hours <= 40; }

float getInput(bool (*validateFunction)(float)) {
  float input;

  std::cin >> input;

  while (std::cin.fail() || !validateFunction(input)) {
    display("Pay outside of range, try again", true);

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }
  return input;
}

int getInput(bool (*validateFunction)(int)) {
  int input;

  std::cin >> input;

  while (std::cin.fail() || !validateFunction(input)) {
    display("Hours outside of range, try again", true);

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }
  return input;
}

String formatFloat(float input) {
  const float roundedInput = round(input * 100) / 100;
  const String stringInput = std::to_string(roundedInput);
  const int index = stringInput.find(".");
  const String substringInput =
      stringInput.substr(0, index) + "." + stringInput.substr(index + 1, 2);
  return substringInput;
}

int main() {
  display("Enter hourly pay rate: ", 1);
  const float payRate = getInput(&isValidPay);

  display("Enter hours worked: ", 1);
  const int hours = getInput(&isValidHours);

  const float grossPay = payRate * hours;
  const String grossPayString = formatFloat(grossPay);

  display("Gross Pay: $" + grossPayString, 1);
}
