#include <iostream>
#include <string>

// Adds commas and $ sign to input; ie: 1234567 => $1,234,567

void display(std::string, bool = false);
std::string formatUsd(std::string);

int main() {
  std::string input;
  display("Enter dollars: ");
  std::cin >> input;
  std::cout << formatUsd(input) << std::endl;
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

std::string formatUsd(std::string input) {
  for (int i = input.size() - 3; i > 0; i = i - 3) {
    input.insert(i, ",");
  }

  return "$" + input;
}

std::string formatUsdOld(int input) {
  const std::string usdString = std::to_string(input);
  std::string usdFormatted;

  const int commas = (usdString.size() - 1) / 3;

  for (int i = 0; i < commas + 1; i++) {
    const std::string usdChunk =
        i == commas ? "$" + usdString.substr(0, usdString.size() - i * 3)
                    : usdString.substr(usdString.size() - (i + 1) * 3, 3);

    usdFormatted = i == 0 ? usdChunk : usdChunk + "," + usdFormatted;
  }

  return usdFormatted;
}
