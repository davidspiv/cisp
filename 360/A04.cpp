/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 4
# Date:             9/20/24
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

#include "valid.h"
#include <iostream>
#include <string>

using namespace std;

// util
string toLowercase(string input);
string formatDouble(double yearsAsDouble);

// input
int getMenuChoice();
double getTargetDistance();
int getVelocity();
int getGeocentricOrigin();

// output
void display(const string &output, bool carriageReturn = 1);
void displayMenu();
void displayTravelTime(const string &resultLabel, double totalYears);

// business logic
double calcYears(int geocentricOrigin, double targetDistance, int velocity);
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
   double totalYears = 0.0;

   display("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {
         totalYears += handleTravelTime();
      }

   } while (menuChoice != QUIT);

   displayTravelTime("Total time", totalYears);
   display("\nEnd program.");

   return 0;
}


// display() outputs value to console
// Pre: output is a string, carriageReturn is a bool
// Post: output is displayed and output stream buffer flushed
void display(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }
   cout << output << flush;
}


// displayMenu() shows menu choices that correspond to integer constants
// Pre: none
// Post: menu is displayed
void displayMenu()
{
   display("\n1. Add waypoint");
   display("2. Quit");
}


// getMenuChoice() gets menu option and reurns it
// Pre: none
// Post: returns integer between 0 and 3 noninclusive
int getMenuChoice()
{
   int menuOption = 0;

   displayMenu();
   menuOption = getInteger("Choose option: ");

   while (menuOption <= 0 || menuOption > 2) {
      display("Must choose number from menu options.");
      displayMenu();
      menuOption = getInteger("Choose option: ");
   }

   return menuOption;
}


// toLowercase() will transform any uppercase letter to lowercase
// Pre: input must be string
// Post: input is returned as all lowercase string
string toLowercase(string input)
{
   for (size_t charIndex = 0; charIndex < input.length(); charIndex++) {
      input[charIndex] = tolower(input[charIndex]);
   }

   return input;
}


// getTargetDistance() matches a user input with a list of celestial bodies,
// returns the distance from earth associated with each body
// Pre: none
// Post: targetDistance is a double
double getTargetDistance()
{
   double targetDistance = 0;
   string planet = "";

   while (targetDistance < 1) {

      planet = getString("Enter a planet in our solar system: ");
      planet = toLowercase(planet);

      if (planet.length() > 10) {
         display("Planet must be within our solar system.");
      }
      else if (planet.find("mercury") != string::npos) {
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
         display("Planet must be within our solar system.");
      }
   }

   return targetDistance;
}


// getGeocentricOrigin() gets a user input is miles from earth and returns it
// Pre: none
// Post: geocentricOrigin is an integer in between 0 and 100K
int getGeocentricOrigin()
{
   int geocentricOrigin = getInteger("\nEnter origin (miles from Earth): ");

   while (geocentricOrigin <= 0 || geocentricOrigin >= 100000) {
      display("Must be greater than 0 and less than 100K miles.");
      geocentricOrigin = getInteger("Enter origin (miles from Earth): ");
   }

   return geocentricOrigin;
}


// getVelocity() gets a user input is miles per hour and returns it
// Pre: none
// Post: velocityAsMph is an integer greater than 0
int getVelocity()
{
   int velocityAsMph = getInteger("Enter miles per hour: ");

   while (velocityAsMph <= 0) {
      display("Must be greater than 0.");
      velocityAsMph = getInteger("Enter miles per hour: ");
   }

   return velocityAsMph;
}


// calcYears() will calculate the distance between an origin and a target,
// returns the time it takes to get there
// Pre: milesFromEarth is an int, targetDistance is a double, velocityAsMph is
// an int
// Post: a positive integer is returned
double calcYears(int geocentricOrigin, double targetDistance, int velocity)
{
   const double distanceInMiles = abs(targetDistance - geocentricOrigin);
   const double years = distanceInMiles / velocity / HOURS_PER_YEAR;

   return years;
}


// formatDouble() will convert a double to a comma separated value truncated to
// two decimal places
// Pre: yearsAsDouble is a double
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
// Pre: resultLabel is a const string ref, totalYears is double
// Post: formatted result is displayed and accurately represents computed value
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


// handleTravelTime() calculates the time scalar to celestial bodies
// Pre: none
// Post: years returned as a double
double handleTravelTime()
{
   const int geocentricOrigin = getGeocentricOrigin();
   const double targetDistance = getTargetDistance();
   const int velocity = getVelocity();

   const double years = calcYears(geocentricOrigin, targetDistance, velocity);

   displayTravelTime("Travel time", years);
   display("");

   return years;
}
