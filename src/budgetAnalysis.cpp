#include <iostream>
#include <string>

void display(std::string);
int getInput();
int getTransactionSum();
std::string getResultString(int);
std::string formatUsd(int);
bool isOverflow(int);
void endProgram(std::string);

int main() {
  int budgetInput, transactionSum, budgetDiff;
  std::string resultString;

  std::cout << "Enter this month's budget: ";

  budgetInput = getInput();

  std::cout <<
      R"(Type an integer and press enter to input a transaction amount.
Enter as many transactions as you'd like.
Then enter 0 to initiate the calculation.
)";

  transactionSum = getTransactionSum();
  budgetDiff = budgetInput - transactionSum;
  resultString = getResultString(budgetDiff);

  std::cout << (resultString + "\n");
}

int getInput() {
  int input;
  std::cin >> input;

  if (isOverflow(input)) {
    endProgram("Budget input outside of integer range");
  };
  return input;
}

int getTransactionSum() {
  int userInput, total = 0;
  std::cin >> userInput;
  while (userInput) {
    if (isOverflow(userInput)) {
      std::cout << "Transaction outside of integer range, not included";
      std::cin.clear();
    } else {
      total += userInput;
    }
    std::cin >> userInput;
  }

  if (isOverflow(total)) endProgram("Transaction sum outside of integer range");
  return total;
}

std::string getResultString(int budgetDiff) {
  std::string resultStringComponent = " under ";

  if (budgetDiff < 0) {
    budgetDiff *= -1;
    resultStringComponent = " over ";
  }

  return "You are " + formatUsd(budgetDiff) + resultStringComponent + "budget.";
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

void endProgram(std::string err) {
  std::cout << err + "\n";
  exit(1);
}
