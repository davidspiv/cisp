#include <iostream>

void display(std::string, int = 0);
int getInput();
int getTransactionSum();
std::string getResultString(int);
std::string formatUsd(int);
bool isOverflow(int);
void error(std::string);

int main() {
  int budgetInput, transactionSum, budgetDiff;
  std::string resultString;

  display("Enter this month's budget: ");

  budgetInput = getInput();

  display("Type an integer and press enter to input a transaction amount.", 1);
  display("Enter as many transactions as you'd like.", 1);
  display("Then enter \"0\" to initiate the budget analysis calculation.", 1);

  transactionSum = getTransactionSum();
  budgetDiff = budgetInput - transactionSum;
  resultString = getResultString(budgetDiff);

  display(resultString, 1);
}

void display(std::string input, int returnFlg) {
  if (returnFlg) {
    std::cout << input << std::endl;
    return;
  }
  std::cout << input;
}

int getInput() {
  int input;
  std::cin >> input;

  if (isOverflow(input)) {
    error("Budget input outside of integer range");
  };
  return input;
}

int getTransactionSum() {
  int userInput = 1, total = 0;

  while (userInput) {
    std::cin >> userInput;
    if (isOverflow(userInput)) {
      error("Transaction outside of integer range");
    }
    total += userInput;
  }

  if (isOverflow(total)) error("Transaction sum outside of integer range");
  return total;
}

std::string getResultString(int budgetDiff) {
  std::string resultStringComponent;

  if (budgetDiff < 0) {
    budgetDiff = budgetDiff * -1;
    resultStringComponent = " over your budget.";
  } else {
    resultStringComponent = " under your budget.";
  }

  return "You are " + formatUsd(budgetDiff) + resultStringComponent;
}

std::string formatUsd(int input) {
  std::string usdString, hundreds, thousands, millions, usdFormatted;

  usdString = std::to_string(input);
  millions = thousands = "";

  if (usdString.size() > 6) {
    millions = usdString.substr(0, usdString.size() - 6) + ",";
    usdString = usdString.substr(usdString.size() - 6, usdString.size());
  }

  if (usdString.size() > 3) {
    thousands = usdString.substr(0, usdString.size() - 3) + ",";
    usdString = usdString.substr(usdString.size() - 3, usdString.size());
  }

  hundreds = usdString;
  usdFormatted = "$" + millions + thousands + hundreds;
  return usdFormatted;
}

bool isOverflow(int input) {
  return input == 2147483647 || input == -2147483648;
}

void error(std::string err) {
  display(err, 1);
  exit(1);
}
