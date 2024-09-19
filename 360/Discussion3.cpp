/******************************************************************************
# Author:           David Spivack
# Discussion:       3
# Date:             9/14/24
# Description:      Output a coin breakdown from a user input of total pennies.
# Sources:          Assignment 3 specifications
# Comments:         I struggled to isolate the calculation and the display
#                   portion of the program. My first instinct was to have the
#                   calculate function call the display function for each coin
#                   denomination. However, structuring my program so that main
#                   called each function separately was more readable with less
#                   side effects.
#******************************************************************************/

#include <iostream>
#include <numeric>
#include <string>

using namespace std;


// outputs value from first argument to console and flushes the output stream
// buffer either via std::endl or std::flush depending on the second argument.
void display(const string &output, bool carriageReturn = 1)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }

   cout << output << flush;
}


// gets an integer from the user and loops if it is not at or above 0 or below
// the overflow limit of an integer.
int getChange()
{
   int cents = 0;

   display("\nEnter in your total change in cents: ", 0);
   cin >> cents;

   while (cin.fail() || cents < 0) {
      cout << endl
           << "Must be a number between 0 and 2147483647 inclusive. Try "
              "again: ";

      cin.clear();
      cin.ignore(1000, '\n');
      cin >> cents;
   }

   display("\n", 0);

   return cents;
}


// displays formatted output line specific to the denomination
void displayCoinValue(int coinValue, int denominator)
{
   const string coinQuantityAsString = to_string(coinValue / denominator);

   string message = "";
   string coinTypeAsString = "";

   switch (denominator) {
   case 100:
      coinTypeAsString = " Dollar";
      break;
   case 25:
      coinTypeAsString = " Quarter";
      break;
   case 10:
      coinTypeAsString = " Dime";
      break;
   case 5:
      coinTypeAsString = " Nickle";
      break;
   default:
      coinTypeAsString = " Cent";
      break;
   }

   if (coinValue > 0) {
      message += coinQuantityAsString + coinTypeAsString;
   }

   if (coinValue > denominator) {
      message += "s";
   }

   if (message != "") {
      display(message);
   }
}


// calculates the value (as cents) of the maximum number of a specified coin
// type that can be used
int calcCoinValue(int cents, int denominator)
{
   return cents - cents % denominator;
}


// pulls coin logic out of main
void handleCoins(int change)
{
   const int coins[] = {100, 25, 10, 5, 1};

   for (int coin : coins) {
      const int coinValue = calcCoinValue(change, coin);
      displayCoinValue(coinValue, coin);

      change -= coinValue;
   }
}


int main()
{
   int change = 0;

   display("Welcome!");

   change = getChange();

   if (change > 0) {
      handleCoins(change);
   }
   else {
      display("No Change.");
   }

   display("\nGoodbye!");

   return 0;
}
