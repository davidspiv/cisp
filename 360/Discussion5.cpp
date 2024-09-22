/******************************************************************************
# Author:           David Spivack
# Discussion:       5
# Date:             9/19/24
# Sources:          Discussion 5 specifications
# Comments:         I struggled with passing the array back and forth between
#                   functions amd choosing between passing by pointer vs
#                   passing by reference. I ended up passing the array size as
#                   a second argument for most functions because I didn't want
#                   to have a global variable.
#******************************************************************************/

#include <algorithm>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

void print(const string &output, bool carriageReturn = 1);
bool isDuplicate(int target, int arr[], size_t arrSize);
int getRandomDigit(int arr[], size_t arrSize);
int getLotteryDigit(int picks[], size_t picksSize);


int main()
{
   const size_t LOTTERY_SIZE = 5;

   int lottery[LOTTERY_SIZE];
   int picks[LOTTERY_SIZE];
   int totalMatches = 0;

   string endGameMessage = R"(
You lose, but your government wins! A lose-win is better than a
lose-lose.
)";

   srand(time(0));

   print("Welcome to government-funded gambl - err I mean the lottery!");

   print(R"(
Each digit in the winning number is unique and is position
agnostic, so it doesn't matter if your digits are out of order.
)");
   print("Enter " + to_string(LOTTERY_SIZE) + " digits: ");

   for (size_t i = 0; i < LOTTERY_SIZE; i += 1) {
      lottery[i] = getRandomDigit(lottery, LOTTERY_SIZE);
      picks[i] = getLotteryDigit(picks, LOTTERY_SIZE);
   }

   for (int pick : picks) {
      if (isDuplicate(pick, lottery, LOTTERY_SIZE)) {
         totalMatches += 1;
      };
   }

   if (totalMatches == LOTTERY_SIZE) {
      endGameMessage = "\nYou win and get to share with the "
                       "government! A win-win!";
   }

   print(endGameMessage);
   print("Total matches: " + to_string(totalMatches));
   print("\nEnd program.");

   return 0;
}


// print() outputs string to console
// Pre: output is a string, carriageReturn is a bool
// Post: output is displayed and output stream buffer flushed
void print(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }
   cout << output << flush;
}


// isDuplicate returns true if target matches any array value, false otherwise
// Pre: target is integer to search for, arr is the array to search through as
// a pointer, arrSize is the size of that array
// Post: returns bool value
bool isDuplicate(int target, int arr[], size_t arrSize)
{
   return find(arr, arr + arrSize, target) != arr + arrSize;
}


// getRandomDigit() gets random number between 0 and 9 inclusive and unique to
// the lottery array.
// Pre: lottery is an int array pointer, lotterySize is size of lottery array
// Post: randomDigit is integer
int getRandomDigit(int arr[], size_t arrSize)
{
   int randomDigit = 0;
   do {
      randomDigit = rand() % 10;
   } while (isDuplicate(randomDigit, arr, arrSize));
   return randomDigit;
}


// handleErrors() prints error message if there is one
// Pre: error message as string reference
// Post: error message is printed if needed
void handleErrors(string &errorMessage)
{
   if (errorMessage.length()) {
      print(errorMessage);
      errorMessage = "";
   }
}


// getLotteryDigit gets an integer between 1 and 9 that is unique
// Pre: picks is an array pointer to all other valid input user entered
// at runtime, picksSize is array size
// Post: lotteryDigit is int
int getLotteryDigit(int picks[], size_t picksSize)
{
   int lotteryDigit = 0;
   string errorMessage = "";

   do {
      handleErrors(errorMessage);

      cin >> lotteryDigit;

      if (!cin || cin.peek() != '\n') {
         cin.clear();
         cin.ignore(1000, '\n');
         errorMessage = "Error: Invalid integer.";
      }
      else if (lotteryDigit < 0 || lotteryDigit > 9) {
         errorMessage = "Error: Integer must be between 0 and 9.";
      }
      else if (isDuplicate(lotteryDigit, picks, picksSize)) {
         errorMessage = "Error: Integer must be unique.";
      }

   } while (errorMessage.length());

   cin.get();
   return lotteryDigit;
}
