#include <iostream>
#include <string>

using String = std::string;

void display(String output, bool carriageRtn = false);
int getInput();
int getSumFromInputs();
bool isOverflow(int addendA, int addendB);
String buildResponseString(int budgetDiff);
String insertCommas(int input);
void endProgram(String err);

int main() {
  display("Enter this month's budget: ");

  const int budgetInput = getInput();

  display(R"(
Type an integer and press enter to input a transaction amount.
Enter as many transactions as you'd like.
Then enter 0 to initiate the calculation.

)");

  const int transactionSum = getSumFromInputs();
  const int budgetDiff = budgetInput - transactionSum;
  const String responseString = buildResponseString(budgetDiff);

  display(responseString, true);
}

void display(String output, bool carriageRtn) {
  if (carriageRtn) output += "\n";

  std::cout << output;
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

int getSumFromInputs() {
  int total = 0;
  int userInput = getInput();

  while (userInput) {
    if (isOverflow(total, userInput)) endProgram("Calc failed");

    total += userInput;
    userInput = getInput();
  }
  return total;
}

bool isOverflow(int addendA, int addendB) {
  const int sum = addendA + addendB;
  if (addendA > 0 && addendB > 0 && sum < 0) return true;
  if (addendA < 0 && addendB < 0 && sum > 0) return true;
  return false;
}

String buildResponseString(int budgetDiff) {
  const String resultStringComponent = budgetDiff < 0 ? " over " : " under ";

  return "Result: $" + insertCommas(abs(budgetDiff)) + resultStringComponent +
         "budget.";
}

String insertCommas(int input) {
  String usdString = std::to_string(input);
  for (int i = usdString.size() - 3; i > 0; i = i - 3) {
    usdString.insert(i, ",");
  }
  return usdString;
}

void endProgram(String err) {
  display(err, true);
  exit(1);
}
