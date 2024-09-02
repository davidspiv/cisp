/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 1
# Date:             8/31/24
# Description:      Calculate the travel time to given planet from a specified
#                   starting point and velocity. Prints result to screen.
# Sources:          Assignment 1 specifications
#                   https://cplusplus.com/reference/string/to_string/
#                   https://en.cppreference.com/w/c/types/size_t
#                   planet distances are relative to the current date and
#                   were sourced from NASA as shown in Google results.
#******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

const int HOURS_PER_YEAR = 8760;
const float MILES_VENUS = 667.00e5;
const float MILES_MERCURY = 765.56e5;
const float MILES_MARS = 133.68e6;
const float MILES_JUPITER = 475.74e6;
const float MILES_SATURN = 804.92e6;
const float MILES_NEPTUNE = 269.03e7;
const float MILES_PLUTO = 318.87e7;


// inputInt() gets an integer from the user and loop if it is not above 0 or
// below the overflow limit of an integer.
// Pre: none
// Post: the input is returned
int inputInt()
{
   int input;

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
// Post: the lowercase input is returned
string toLowercase(string input)
{
   for (size_t charIndex = 0; charIndex < input.length(); charIndex++) {
      input[charIndex] = tolower(input[charIndex]);
   }

   return input;
}


// getTargetDistance() gets input from the user and loops until the input
// matches one of the predetermined planets. Returns the distance from earth
// associated with each planet.
// Pre: none
// Post: targetDistance is returned
// Notes: Unable to use switch statement with strings. Potentially
// could convert the input string to a number using a hash map, and return that
// instead. That way I could keep this function solely for planet input and
// calculate targetDistance with a switch statement in the parent function.
float getTargetDistance()
{
   float targetDistance = -1;
   string planetInput = "";

   while (targetDistance < 1) {
      cin >> planetInput;

      const string planet = toLowercase(planetInput);

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
         cout << endl << "Planet must be within our solar system. Try again: ";
      }
   }

   return targetDistance;
}


// formatYears() will convert a float to a comma separated value truncated to
// six decimal places.
// Pre: yearsInput is specified as a float but will work with any primitive
// numerical values.
// Post: the formatted string is returned
// Notes: I am using to_string from the standard library which
// always outputs a result with 6 decimal places (provided the input is of the
// correct type). A potentially better way of doing this would be with
// setprecision() on a stringstream. Also, my comma insertion loop is unable to
// use size_t as a type for the digitIndex variable. I suspect this is because
// digitIndex would be unable to hold the negative result of the final loop.
string formatYears(float yearsInput)
{
   string yearsAsString = to_string(yearsInput);

   for (int digitIndex = yearsAsString.length() - 10; digitIndex > 0;
        digitIndex = digitIndex - 3) {
      yearsAsString.insert(digitIndex, ",");
   }

   return yearsAsString;
}


// Calculate the travel time to given planet from a specified starting point
// and velocity. Prints result to screen.
// Pre: none
// Post: yearsFormatted is returned in the output sentence
int main()
{
   cout << "Planet Trip Calculator" << endl << endl;

   cout << "Enter starting point (miles from Earth): ";
   const int milesFromEarth = inputInt();

   cout << endl << "Enter a planet in our solar system: ";
   const float targetDistance = getTargetDistance();

   cout << endl << "Enter MPH: ";
   const int mph = inputInt();
   const float distanceInMiles = abs(targetDistance - milesFromEarth);
   const float years = distanceInMiles / mph / HOURS_PER_YEAR;
   const string yearsFormatted = formatYears(years);

   cout << endl
        << "It will take you " + yearsFormatted +
               " year(s) to reach your destination."
        << endl
        << endl
        << "End program." << endl;
   ;

   return 0;
}
