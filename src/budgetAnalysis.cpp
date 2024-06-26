#include <iostream>
#include <string>

bool isOverflow(int input) {
  return input == 2147483647 || input == -2147483648;
}

void endProgram(std::string err) {
  std::cout << err + "\n";
  exit(1);
}

int getInput() {
  int input;
  std::cin >> input;

  if (isOverflow(input)) {
    endProgram("Budget input outside of integer range");
  };
  return input;
}

std::string formatUsd(int input) {
  std::string usdFormatted = "$";

  const std::string usdStr = std::to_string(input);
  const int commas = (usdStr.size() - 1) / 3;

  if (commas) {
    const int remainder = usdStr.size() % (commas * 3);
    const int firstCommaIndex = remainder ? remainder : 3;

    const std::string largestChunk = usdStr.substr(0, firstCommaIndex) + ",";
    usdFormatted += largestChunk;

    for (int i = 1; i < commas; i++) {
      const std::string anonChunk =
          usdStr.substr(firstCommaIndex + (i - 1) * 3, 3) + ",";
      usdFormatted += anonChunk;
    }
  }

  const std::string hundredsChunk =
      usdStr.size() > 3 ? usdStr.substr(usdStr.size() - 3, 3) : usdStr;

  usdFormatted += hundredsChunk;

  return usdFormatted;
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

  return "Result: " + formatUsd(budgetDiff) + resultStringComponent + "budget.";
}

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
