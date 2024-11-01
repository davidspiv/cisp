#include <iostream>

void displayPrompt();
int getUserInput();
std::string centsToDollars(int);

int main()
{
  int userInput;

  displayPrompt();
  userInput = getUserInput();
  std::cout << centsToDollars(userInput) << std::endl;
  return 0;
}

void displayPrompt()
{
  std::cout << "Enter amount in cents.";
}

int getUserInput()
{
  int userInput;
  std::cin >> userInput;

  return userInput;
}

std::string centsToDollars(int result)
{
  std::string resultString, usDollars, usCents, formattedResultTotal;
  int dollarLength;

  resultString = std::to_string(result);
  usDollars = resultString.substr(0, resultString.size() - 2);
  usCents = resultString.substr(resultString.size() - 2, 2);
  dollarLength = usDollars.length();

  if (dollarLength > 6)
  {
    std::string millions, thousands, hundreds;
    millions = usDollars.substr(0, usDollars.size() - 6);
    thousands = usDollars.substr(usDollars.size() - 6, usDollars.size() - 4);
    hundreds = usDollars.substr(usDollars.size() - 3, usDollars.size());
    formattedResultTotal = "$" + millions + "," + thousands + "," + hundreds + "." + usCents;
  }
  else if (dollarLength > 3)
  {
    std::string thousands, hundreds;
    thousands = usDollars.substr(0, usDollars.size() - 3);
    hundreds = usDollars.substr(usDollars.size() - 3, usDollars.size());
    formattedResultTotal = "$" + thousands + "," + hundreds + "." + usCents;
  }
  else
  {
    formattedResultTotal = "$" + usDollars + "." + usCents;
  }

  return formattedResultTotal;
}
