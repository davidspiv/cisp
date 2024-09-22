
#include "valid.h"
#include <iostream>
#include <numeric>
#include <string>

using namespace std;

const int ADD_WAYPOINT = 1;
const int QUIT = 2;

const int HOURS_PER_YEAR = 8760;
const double MILES_PER_AU = 9.296e+7;

const double VENUS_DISTANCE_AS_AU = 1.42;
const double MERCURY_DISTANCE_AS_AU = 1.29;
const double MARS_DISTANCE_AS_AU = 1.31;
const double JUPITER_DISTANCE_AS_AU = 4.83;
const double SATURN_DISTANCE_AS_AU = 8.68;
const double URANUS_DISTANCE_AS_AU = 19.04;
const double NEPTUNE_DISTANCE_AS_AU = 28.89;
const double PLUTO_DISTANCE_AS_AU = 34.56;

void print(const string &output, bool carriageReturn = 1)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }
   cout << output << flush;
}

void printMenu()
{
   print("\n1. Add waypoint");
   print("2. Quit");
}

int getMenuChoice()
{
   int menuOption = 0;

   printMenu();
   menuOption = getInteger("Choose option: ");

   while (menuOption <= 0 || menuOption > 2) {
      print("Must choose number from menu options.");
      printMenu();
      menuOption = getInteger("Choose option: ");
   }

   return menuOption;
}

string toLowercase(string input)
{
   for (size_t charIndex = 0; charIndex < input.length(); charIndex += 1) {
      input[charIndex] = tolower(input[charIndex]);
   }

   return input;
}

double getTargetDistance()
{
   double targetDistance = 0;
   string planet = "";

   while (targetDistance < 1) {

      planet = getString("Enter a planet in our solar system: ");
      planet = toLowercase(planet);

      if (planet.length() > 10) {
         print("Planet must be within our solar system.");
      }
      else if (planet.find("mercury") != string::npos) {
         targetDistance = MERCURY_DISTANCE_AS_AU;
      }
      else if (planet.find("venus") != string::npos) {
         targetDistance = VENUS_DISTANCE_AS_AU;
      }
      else if (planet.find("mars") != string::npos) {
         targetDistance = MARS_DISTANCE_AS_AU;
      }
      else if (planet.find("jupiter") != string::npos) {
         targetDistance = JUPITER_DISTANCE_AS_AU;
      }
      else if (planet.find("uranus") != string::npos) {
         targetDistance = URANUS_DISTANCE_AS_AU;
      }
      else if (planet.find("saturn") != string::npos) {
         targetDistance = SATURN_DISTANCE_AS_AU;
      }
      else if (planet.find("pluto") != string::npos) {
         targetDistance = PLUTO_DISTANCE_AS_AU;
      }
      else if (planet.find("neptune") != string::npos) {
         targetDistance = NEPTUNE_DISTANCE_AS_AU;
      }
      else {
         print("Planet must be within our solar system.");
      }
   }

   return targetDistance * MILES_PER_AU;
}

int getGeocentricOrigin()
{
   int geocentricOrigin = getInteger("\nEnter origin (miles from Earth): ");

   while (geocentricOrigin <= 0 || geocentricOrigin >= 100000) {
      print("Must be greater than 0 and less than 100K miles.");
      geocentricOrigin = getInteger("Enter origin (miles from Earth): ");
   }

   return geocentricOrigin;
}

int getVelocity()
{
   int velocityAsMph = getInteger("Enter miles per hour: ");

   while (velocityAsMph <= 0) {
      print("Must be greater than 0.");
      velocityAsMph = getInteger("Enter miles per hour: ");
   }

   return velocityAsMph;
}

double calcYears(int geocentricOrigin, double targetDistance, int velocity)
{
   const double distanceInMiles = abs(targetDistance - geocentricOrigin);
   const double years = distanceInMiles / velocity / HOURS_PER_YEAR;

   return years;
}

string formatDouble(double yearsAsDouble)
{
   string yearsAsString = to_string(yearsAsDouble);
   yearsAsString = yearsAsString.substr(0, yearsAsString.length() - 4);

   for (int i = yearsAsString.length() - 6; i > 0; i = i - 3) {
      yearsAsString.insert(i, ",");
   }

   return yearsAsString;
}

string formatTravelTime(const string &label, double totalYears)
{
   string formattedTime = label + ": ";
   string resultBorder = "";

   if (totalYears == 0) {
      formattedTime += "None";
   }
   else if (totalYears == 1) {
      formattedTime += "1 year";
   }
   else if (totalYears < 1) {
      formattedTime += to_string(totalYears) + " of a year";
   }
   else if (totalYears > 100000) {
      formattedTime += formatDouble(totalYears / 1000) + " millennia";
   }
   else {
      formattedTime += formatDouble(totalYears) + " years";
   }

   // resultBorder should be the same length as the result
   for (size_t i = 0; i < formattedTime.length(); i += 1) {
      resultBorder += "-";
   }

   return "\n" + resultBorder + "\n" + formattedTime + "\n" + resultBorder;
}

void printWaypointTime(int &origin, double &distance, int &velocity)
{
   double years = 0;
   string formattedTime = "";

   origin = getGeocentricOrigin();
   distance = getTargetDistance();
   velocity = getVelocity();

   years = calcYears(origin, distance, velocity);
   formattedTime = formatTravelTime("Travel time", years);

   print(formattedTime);
}

void printTotalTime(int *origins, double *distances, int *velocities)
{
   int validIndex = 0;
   double years = 0;
   int sum = 0;
   string formattedTime = "";

   while (origins[validIndex] != 0 && distances[validIndex] != 0 &&
          velocities[validIndex] != 0) {

      years = calcYears(origins[validIndex], distances[validIndex],
                        velocities[validIndex]);
      sum += years;
      validIndex += 1;
   }

   formattedTime = formatTravelTime("Total time", sum);
   print(formattedTime);
}

int main()
{
   const int MAX = 10;
   int origins[MAX] = {};
   double distances[MAX] = {};
   int velocities[MAX] = {};
   int menuChoice = 0;
   int inputCount = 0;

   print("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {

         if (inputCount == MAX) {
            print("Add array slots");
         }
         else {
            printWaypointTime(origins[inputCount], distances[inputCount],
                              velocities[inputCount]);
            inputCount += 1;
         }
      }

   } while (menuChoice != QUIT);

   printTotalTime(origins, distances, velocities);
   print("\nEnd program.");

   return 0;
}
