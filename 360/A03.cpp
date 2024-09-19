/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 3
# Date:             9/16/24
# Description:      The user will be presented with a menu showing options to
#                   “add a waypoint”, “show total travel time”, or “quit”. When
#                   adding a waypoint, the user enters the origin point, name
#                   of a planet, and miles per hour. The travel time as
#                   years will be added to a total and displayed when the user
#                   chooses the relevant menu option.
# Sources:          Assignment 3 specifications
#                   planet distances are relative to the current date and
#                   were sourced from NASA in early August 2024.
#******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

void display(const string &output, bool carriageReturn = 1);
void displayMenu();
int getInteger(const string &inputPrompt);
string toLowercase(string input);
double getTargetDistance();
double calcYears(int milesFromEarth, double targetDistance, int velocityAsMph);
string formatDouble(double yearsAsDouble);
void displayTravelTime(const string &resultLabel, double totalYears);
double handleTravelTime();

const int ADD_WAYPOINT = 1;
const int QUIT = 2;
const int HOURS_PER_YEAR = 8760;
const double MILES_VENUS = 667.00e5;
const double MILES_MERCURY = 765.56e5;
const double MILES_MARS = 133.68e6;
const double MILES_JUPITER = 475.74e6;
const double MILES_SATURN = 804.92e6;
const double MILES_NEPTUNE = 269.03e7;
const double MILES_PLUTO = 318.87e7;


int main()
{
   int menuChoice = 0;
   float years = 0.0;
   float totalYears = 0.0;

   display("Planet Trip Calculator");

   do {

      displayMenu();
      menuChoice = getInteger("Enter a choice: ");

      if (menuChoice == ADD_WAYPOINT) {
         years = handleTravelTime();
         displayTravelTime("Travel time", years);

         totalYears += years;
      }

   } while (menuChoice != QUIT);

   displayTravelTime("Total time", totalYears);
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


// displayMenu() shows menu choices that correspond to integer constants.
// Pre: menu choices reflect programs capabilities
// Post: Menu is displayed
void displayMenu()
{
   display("");
   display("1. Add waypoint");
   display("2. Quit");
}


// getInteger() displays an input prompt via its only argument. Then gets an
// integer from the user and loop if it is not at or above 0 or below the
// overflow limit of an integer.
// Pre: inputPrompt is accessed via immutable reference
// Post: the input is returned
int getInteger(const string &inputPrompt)
{
   int input = 0;

   display(inputPrompt, 0);
   cin >> input;

   while (cin.fail() || input < 0) {
      display("Must be a number between 0 and 2147483647 inclusive. Try "
              "again: ",
              0);

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
// should be changed.
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

   display("Enter a planet in our solar system: ", 0);

   while (targetDistance < 1) {

      cin >> planet;
      planet = toLowercase(planet);

      if (planet.find("mercury") != string::npos) {
         targetDistance = MILES_MERCURY;
      }
      else if (planet.find("venus") != string::npos) {
         targetDistance = MILES_VENUS;
      }
      else if (planet.find("mars") != string::npos) {
         targetDistance = MILES_MARS;
      }
      else if (planet.find("jupiter") != string::npos) {
         targetDistance = MILES_JUPITER;
      }
      else if (planet.find("saturn") != string::npos) {
         targetDistance = MILES_SATURN;
      }
      else if (planet.find("pluto") != string::npos) {
         targetDistance = MILES_PLUTO;
      }
      else if (planet.find("neptune") != string::npos) {
         targetDistance = MILES_NEPTUNE;
      }
      else {
         display("Planet must be within our solar system. Try again: ", 0);

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
// and milesFromEarth must be between 0 and 2147483647 inclusive
// Post: a positive integer is returned
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

   for (int i = yearsAsString.length() - 6; i > 0; i = i - 3) {
      yearsAsString.insert(i, ",");
   }

   return yearsAsString;
}


// displayTravelTime() formats and prints result message
// Pre: totalYears as double
// Post: formatted result accurately represents computed value and is displayed
void displayTravelTime(const string &resultLabel, double totalYears)
{
   string resultAsString = resultLabel + ": ";
   string resultBorder = "";

   if (totalYears == 0) {
      resultAsString += "None!";
   }
   else if (totalYears == 1) {
      resultAsString += "1 year";
   }
   else if (totalYears < 1) {
      resultAsString += to_string(totalYears) + " of a year";
   }
   else if (totalYears > 100000) {
      resultAsString += formatDouble(totalYears / 1000) + " millennia";
   }
   else {
      resultAsString += formatDouble(totalYears) + " years";
   }

   // resultBorder should be the same length as the result
   for (size_t i = 0; i < resultAsString.length(); i++) {
      resultBorder += "-";
   }

   display("");
   display(resultBorder);
   display(resultAsString);
   display(resultBorder);
}


// handleTravelTime() gets relevant inputs and calculates years per waypoint.
// Pre: None
// Post: calcYears is called with correct arguments as received from input
// functions. Result is returned from function
double handleTravelTime()
{
   const int milesFromEarth =
       getInteger("\nEnter starting point (miles from Earth): ");
   const double targetDistance = getTargetDistance();
   const int velocityAsMph = getInteger("Enter miles per hour: ");

   return calcYears(milesFromEarth, targetDistance, velocityAsMph);
}
