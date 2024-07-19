#include <iostream>
#include <string>

using String = std::string;

void display(String output, bool returnFlag = false);
long int getInput();
String convertHkdToUSCents(long int hkd);
String insertCommas(String input);
[[noreturn]] void isError(const String &err);

int main() {
  display("Enter HK$: ");

  const long int input = getInput();
  const String totalCents = convertHkdToUSCents(input);
  const int len = totalCents.length();
  const String dollarsSub = len > 2 ? totalCents.substr(0, len - 2) : "0";
  const String centsSub = totalCents.substr(len - 2, 2);

  display("Result  $: " + insertCommas(dollarsSub) + "." + centsSub, true);
}

void display(String output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

long int getInput() {
  long int input;

  std::cin >> input;

  while (std::cin.fail()) {
    display("Input outside of range, try again", true);

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cin >> input;
  }
  return input;
}

String convertHkdToUSCents(long int hkd) {
  const long int result = hkd * 1000 / 78;
  if (result < 0) isError("Calc failed, result too large");
  return std::to_string(result);
}

String insertCommas(String usd) {
  for (int i = usd.size() - 3; i > 0; i = i - 3) {
    usd.insert(i, ",");
  }
  return usd;
}

[[noreturn]] void isError(const String &err) {
  display(err, true);
  exit(1);
}
