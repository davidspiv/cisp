#include <iostream>

int getClassCount(char);
void display(std::string, int = 0);
int calcTotal(int, int, int);
std::string formatPrice(int);
void isError(std::string);

int main() {
  int classACount, classBCount, classCCount, totalPrice;
  std::string formattedPrice;

  classACount = getClassCount('A');
  classBCount = getClassCount('B');
  classCCount = getClassCount('C');

  totalPrice = calcTotal(classACount, classBCount, classCCount);
  formattedPrice = formatPrice(totalPrice);
  display(formattedPrice, 1);

  return 0;
}

int getClassCount(char classLetter) {
  int count;
  std::string classString(1, classLetter);

  display("Number of " + classString + " class tickets: ");
  std::cin >> count;
  if (!count) isError("Did not enter an integer greater than 0.");
  return count;
}

void display(std::string output, int returnFlag) {
  if (returnFlag) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << output;
}

int calcTotal(int a, int b, int c) {
  int classACost = 15;
  int classBCost = 12;
  int classCCost = 9;
  return a * classACost + b * classBCost + c * classCCost;
}

std::string formatPrice(int result) {
  std::string resultString, hundreds, thousands, millions, formattedResult;

  resultString = std::to_string(result);
  millions = thousands = "";

  if (resultString.size() > 6) {
    millions = resultString.substr(0, resultString.size() - 6) + ",";
    resultString =
        resultString.substr(resultString.size() - 6, resultString.size());
  }

  if (resultString.size() > 3) {
    thousands = resultString.substr(0, resultString.size() - 3) + ",";
    resultString =
        resultString.substr(resultString.size() - 3, resultString.size());
  }

  hundreds = resultString;

  formattedResult = "$" + millions + thousands + hundreds;

  return formattedResult;
}

void isError(std::string err) {
  display(err, 1);
  exit(1);
}
