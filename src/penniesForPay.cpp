#include <iostream>

void display(std::string, int = 0);
std::string getResult(int);
void isError(std::string);

int main() {
  int userInput;
  std::string result;

  display("Enter number of days: ");
  std::cin >> userInput;

  if (!userInput) isError("Did not enter an integer greater than 0.");

  result = getResult(userInput);
  display(result, 1);
}

void display(std::string output, int returnFlag) {
  if (returnFlag) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << output;
}

std::string getResult(int input) {
  int calculate(int);
  std::string formatResult(int);

  int result = calculate(input);
  return formatResult(result);
}

int calculate(int input) {
  int iterator = input;
  int loopResult = 0;
  while (iterator > 0) {
    int pennies = input - iterator + 1;
    loopResult += pennies;
    --iterator;
  }
  return loopResult;
}

std::string formatResult(int resultNumber) {
  std::string resultString, dollarsString, centsString;
  int resultSize;

  resultString = std::to_string(resultNumber);
  resultSize = resultString.size();
  if (resultSize < 2) {
    return "$00.0" + resultString;
  } else if (resultSize < 3) {
    return "$00." + resultString;
  }
  dollarsString = resultString.substr(0, resultString.size() - 2);
  centsString =
      resultString.substr(resultString.size() - 2, resultString.size());
  return "$" + dollarsString + "." + centsString;
}

void isError(std::string err) {
  display(err, 1);
  exit(1);
}
