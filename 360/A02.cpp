/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 2
# Date:             9/12/24
# Description:      Calculate the travel time to given planet from a specified
#                   starting point and velocity. Prints result to screen.
# Sources:          Assignment 2 specifications
#                   https://cplusplus.com/reference/string/to_string/
#                   https://en.cppreference.com/w/c/types/size_t
#                   planet distances are relative to the current date and
#                   were sourced from NASA as shown in Google results.
#******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

void display(const string &output, bool carriageReturn = 1);
int getInteger(const string &inputPrompt);
string toLowercase(string input);
double getTargetDistance();
double calcYears(int milesFromEarth, double targetDistance, int velocityAsMph);
string formatDouble(double yearsAsDouble);

const int HOURS_PER_YEAR = 8760;
const double MILES_VENUS = 667.00e5;
const double MILES_MERCURY = 765.56e5;
const double MILES_MARS = 133.68e6;
const double MILES_JUPITER = 475.74e6;
const double MILES_SATURN = 804.92e6;
const double MILES_NEPTUNE = 269.03e7;
const double MILES_PLUTO = 318.87e7;


// main() calculate the travel time to given planet from a specified starting
// point and velocity. Prints result to screen. Pre: none Post: End program is
// displayed
int main()
{
   int milesFromEarth = 0;
   double targetDistance = 0.0;
   int velocityAsMph = 0;
   double years = -1;
   string yearsFormatted = "";

   display("Planet Trip Calculator");

   milesFromEarth = getInteger("\nEnter starting point (miles from Earth): ");
   targetDistance = getTargetDistance();
   velocityAsMph = getInteger("\nEnter miles per hour: ");

   years = calcYears(milesFromEarth, targetDistance, velocityAsMph);
   yearsFormatted = formatDouble(years);

   display("\nIt will take you " + yearsFormatted +
           " year(s) to reach your destination.\n");

   display("End program.");

   return 0;
}


// display() outputs value to console and flushes the output stream buffer
// either via std::endl or std::flush depending on the second argument.
// Pre: output is an immutable string reference, carriageReturn is a bool
// Post: output is displayed
void display(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }

   cout << output << flush;
}


// getInteger() displays an input prompt passed by argument. Then gets an
// integer from the user and loop if it is not above 0 or below the overflow
// limit of an integer.
// Pre: inputPrompt is passed as an immutable reference
// Post: the input is returned
int getInteger(const string &inputPrompt)
{
   int input = 0;

   display(inputPrompt, 0);

   cin >> input;

   while (cin.fail() || input <= 0) {
      cout << endl << "Must be a number between 0 and 2147483648. Try again: ";

      cin.clear();
      cin.ignore(1000, '\n');
      cin >> input;
   }
   return input;
}


// toLowercase() will transform any uppercase letter to lowercase
// Pre: Input must be string. Will work even if empty or no characters
// applicable
// Post: the lowercase input is returned. No other characters in the string
// should have changed.
string toLowercase(string input)
{
   for (size_t charIndex = 0; charIndex < input.length(); charIndex++) {
      input[charIndex] = tolower(input[charIndex]);
   }

   return input;
}


// getTargetDistance() displays an input prompt. Then gets input from the user
// and loops until the input matches one of the predetermined planets. Returns
// the distance from earth associated with each planet.
// Pre: none
// Post: targetDistance is returned
double getTargetDistance()
{
   double targetDistance = 0;
   string planet = "";

   display("\nEnter a planet in our solar system: ", 0);

   while (targetDistance < 1) {

      cin >> planet;

      planet = toLowercase(planet);

      if (planet == "mercury") {
         targetDistance = MILES_MERCURY;
      }
      else if (planet == "venus") {
         targetDistance = MILES_VENUS;
      }
      else if (planet == "mars") {
         targetDistance = MILES_MARS;
      }
      else if (planet == "jupiter") {
         targetDistance = MILES_JUPITER;
      }
      else if (planet == "saturn") {
         targetDistance = MILES_SATURN;
      }
      else if (planet == "pluto") {
         targetDistance = MILES_PLUTO;
      }
      else if (planet == "neptune") {
         targetDistance = MILES_NEPTUNE;
      }
      else {
         display("\nPlanet must be within our solar system. Try again: ", 0);

         cin.clear();
         cin.ignore(1000, '\n');
      }
   }

   return targetDistance;
}


// calcYears() will calculate the distance between your location and target and
// return the time it takes to get there.
// Pre: milesFromEarth as an int, targetDistance as a double, velocityAsMph as
// an int. None of these can be 0. The absolute difference of targetDistance
// and milesFromEarth can't be equal or greater than 2147483648
// Post: integer is returned
double calcYears(int milesFromEarth, double targetDistance, int velocityAsMph)
{
   const double distanceInMiles = abs(targetDistance - milesFromEarth);
   const double years = distanceInMiles / velocityAsMph / HOURS_PER_YEAR;

   return years;
}


// formatDouble() will convert a double to a comma separated value truncated to
// two decimal places.
// Pre: yearsAsDouble is double
// Post: the formatted string is returned
string formatDouble(double yearsAsDouble)
{
   string yearsAsString = to_string(yearsAsDouble);
   yearsAsString = yearsAsString.substr(0, yearsAsString.length() - 4);

   for (int digitIndex = yearsAsString.length() - 6; digitIndex > 0;
        digitIndex = digitIndex - 3) {
      yearsAsString.insert(digitIndex, ",");
   }

   return yearsAsString;
}
