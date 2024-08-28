#include <iostream>

void display(std::string);
int getUserInput();
bool isValid(int);

int main() {
  int userInput;

  display("enter a charge account number");
  userInput = getUserInput();

  if (!userInput) {
    display("Did not enter an integer greater than 0.");
    return 1;
  }

  if (isValid(userInput)) {
    display("Input is valid");
  } else {
    display("Input is NOT valid");
  };
}

void display(std::string output) { std::cout << output << std::endl; }

int getUserInput() {
  std::string input;
  int formattedInput;
  std::cin >> input;
  try {
    formattedInput = std::stoi(input);
  }
  // returns 0 if no string can be extracted
  catch (std::invalid_argument) {
    return 0;
  }
  return formattedInput;
}

bool isValid(int input) {
  int validationArr[] = {5658845, 4520125, 7895122, 8777541, 8451277, 1302850,
                         8080152, 4562555, 5552012, 5050552, 7825877, 1250255,
                         1005231, 6545231, 3852085, 7576651, 7881200, 4581002};

  for (auto i : validationArr) {
    if (input == i) return true;
  }
  return false;
}
