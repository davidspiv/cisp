#include <iostream>

void display(std::string, int = 0);
int getTransactionSum();
std::string getResultString(int);

int main() {
  int budgetInput, budgetDiff, transactionSum = 0;
  std::string resultString;

  display("Enter this month's budget: ");
  std::cin >> budgetInput;

  transactionSum = getTransactionSum();
  budgetDiff = budgetInput - transactionSum;
  resultString = getResultString(budgetDiff);

  display(resultString, 1);
  return 0;
}

void display(std::string input, int returnFlg) {
  if (returnFlg) {
    std::cout << input << std::endl;
    return;
  }
  std::cout << input;
};

int getTransactionSum() {
  // initialize userInput with value '1' to instigate the while loop
  // probably not good practice since I don't use the value
  int userInput = 1, total = 0;
  display("Enter in all your transaction amounts followed by a '0'", 1);

  while (userInput) {
    std::cin >> userInput;
    total += userInput;
  }

  return total;
};

std::string getResultString(int budgetDiff) {
  std::string resultStringComponent;

  if (budgetDiff < 0) {
    budgetDiff = budgetDiff * -1;
    resultStringComponent = " over your budget.";
  } else {
    resultStringComponent = " under your budget.";
  }

  return "You are $" + std::to_string(budgetDiff) + resultStringComponent;
};
