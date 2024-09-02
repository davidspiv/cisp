#include <iostream>
#include <string>

using String = std::string;

struct Month {
   String name;
   int inches;
};

void display(String output, bool carriageRtn = false);
int getInput();
int setInches(Month month);
std::string addCommas(int input);

int main()
{
   const int numMonths = 12;
   const char *monthNameArr[numMonths] = {
       "january", "february", "march",     "april",   "may",      "june",
       "july",    "august",   "september", "october", "november", "december"};
   Month monthDataArr[numMonths];
   int total, high, low;
   total = high = low = 0;
   display("Enter inches of rainfall per month", 1);

   for (int i = 0; i < numMonths; i++) {
      int inches;
      // populate
      monthDataArr[i].name = monthNameArr[i];
      monthDataArr[i].inches = inches = setInches(monthDataArr[i]);

      // calculate
      total += inches;
      if (!i)
         low = high = inches;
      if (inches > high)
         high = inches;
      if (inches < low)
         low = inches;
   }

   display("\nTOTAL   : " + addCommas(total), 1);
   display("AVERAGE : " + addCommas(total / numMonths), 1);
   display("HIGH    : " + addCommas(high), 1);
   display("LOW     : " + addCommas(low), 1);
}

void display(String output, bool carriageRtn)
{
   if (carriageRtn)
      output += "\n";

   std::cout << output;
}

int getInput()
{
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

int setInches(Month month)
{
   display(month.name + ": ");
   return getInput();
}

std::string addCommas(int input)
{
   std::string inputString = std::to_string(input);
   for (int i = inputString.size() - 3; i > 0; i = i - 3) {
      inputString.insert(i, ",");
   }

   return inputString;
}
