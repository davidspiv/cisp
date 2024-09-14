#include <iostream>
#include <string>
using namespace std;

void display(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }

   cout << output << flush;
}

int calcChange(int val, int change)
{
   return change - change % val;
}

int main()
{
   // const int CHANGE[] = {100, 25, 10, 5, 1};
   string outputMessage = "";
   int cents = 0;
   cout << "Welcome!" << endl;
   cout << "\nEnter in your total change in cents: ";
   cin >> cents;

   int dollars = calcChange(100, cents);
   if (dollars > 0) {
      outputMessage += "\n" + to_string(dollars / 100) + " Dollar";
   }

   if (dollars > 100) {
      outputMessage += "s";
   }

   cents -= dollars;

   int quarters = calcChange(25, cents);
   if (quarters > 0) {
      outputMessage += "\n" + to_string(quarters / 25) + " Quarter";
   }

   if (quarters > 25) {
      outputMessage += "s";
   }

   cents -= quarters;

   int dimes = calcChange(10, cents);
   if (dimes > 0) {
      outputMessage += "\n" + to_string(dimes / 10) + " Dime";
   }

   if (dimes > 10) {
      outputMessage += "s";
   }

   cents -= dimes;

   int nickels = calcChange(5, cents);
   if (nickels > 0) {
      outputMessage += "\n" + to_string(nickels / 5) + " Nickel";
   }

   if (nickels > 5) {
      outputMessage += "s";
   }

   cents -= nickels;

   if (cents > 0) {
      outputMessage += "\n" + to_string(cents) + " Cent";
   }

   if (cents > 1) {
      outputMessage += "s";
   }

   if (outputMessage == "") {
      outputMessage += "No change.";
   }

   cout << outputMessage << endl;
   cout << "\nGoodbye!" << endl;

   return 0;
}
