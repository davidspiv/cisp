#include <iostream>

void display(std::string, int = 0);
int calcDiscount(int);
int calcPrice(int, int);
std::string formatPrice(int);
void isError(std::string);

int main() {
  int packageQtty, discount, totalPrice;
  std::string formattedPrice;
  display("Enter the number of packages purchased: ");
  std::cin >> packageQtty;

  // range maximum is defined by calculation in getPrice()
  if (!packageQtty || packageQtty >= 2147483647 / 9900 / 2)
    isError("Input not within integer range.");

  discount = calcDiscount(packageQtty);
  totalPrice = calcPrice(packageQtty, discount);
  formattedPrice = formatPrice(totalPrice);

  display("Total price: " + formattedPrice, 1);
  display("Discount: " + std::to_string(discount) + "%", 1);

  return 0;
}

void display(std::string output, int returnFlag) {
  if (returnFlag) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << output;
}

int calcDiscount(int qtty) {
  int discount;

  if (qtty < 10) {
    discount = 0;
  } else if (qtty < 20) {
    discount = 20;
  } else if (qtty < 50) {
    discount = 30;
  } else if (qtty < 100) {
    discount = 40;
  } else {
    discount = 50;
  }

  return discount;
}

int calcPrice(int packageQtty, int discount) {
  int packageRetail = 99;
  int total = packageRetail * (100 - discount) * packageQtty;

  return total;
}

std::string formatPrice(int result) {
  std::string resultString, cents, hundreds, thousands, millions,
      formattedResult;

  resultString = std::to_string(result);
  millions = thousands = "";

  if (resultString.size() > 8) {
    millions = resultString.substr(0, resultString.size() - 8) + ",";
    resultString =
        resultString.substr(resultString.size() - 8, resultString.size());
  }

  if (resultString.size() > 5) {
    thousands = resultString.substr(0, resultString.size() - 5) + ",";
    resultString =
        resultString.substr(resultString.size() - 5, resultString.size());
  }

  hundreds = resultString.substr(0, resultString.size() - 2);
  cents = resultString.substr(resultString.size() - 2, 2);
  formattedResult = "$" + millions + thousands + hundreds + "." + cents;

  return formattedResult;
}

void isError(std::string err) {
  display(err, 1);
  exit(1);
}
