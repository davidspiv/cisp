#include <iostream>
#include <string>

void display(std::string, bool = false);
int getInput();
int getSumFromInputs();
std::string formatUsd(int);
std::string buildResponseString(int);
void endProgram(std::string);
bool isOverflow(int);

int main() {
  display("Enter this month's budget: ");

  const int budgetInput = getInput();

  display(R"(Type an integer and press enter to input a transaction amount.
Enter as many transactions as you'd like.
Then enter 0 to initiate the calculation.
)");

  const int transactionSum = getSumFromInputs();
  const int budgetDiff = budgetInput - transactionSum;
  const std::string responseString = buildResponseString(budgetDiff);

  display(responseString);
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

int getInput() {
  int input;

  std::cin >> input;

  while (std::cin.fail()) {
    display("Input outside of integer range, try again\n");

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }

  return input;
}

int getSumFromInputs() {
  int total = 0;
  int userInput = getInput();

  while (userInput) {
    total += userInput;
    if (isOverflow(total))
      endProgram("Calc failed, sum outside of integer range");
    userInput = getInput();
  }
  return total;
}

std::string formatUsd(int input) {
  std::string usdString = std::to_string(input);
  for (int i = usdString.size() - 3; i > 0; i = i - 3) {
    usdString.insert(i, ",");
  }

  return "$" + usdString;
}

std::string buildResponseString(int budgetDiff) {
  const std::string resultStringComponent =
      budgetDiff < 0 ? " over " : " under ";

  return "Result: " + formatUsd(abs(budgetDiff)) + resultStringComponent +
         "budget.\n";
}

void endProgram(std::string err) {
  display(err, 1);
  exit(1);
}

bool isOverflow(int input) {
  return input == 2147483647 || input == -2147483648;
}
