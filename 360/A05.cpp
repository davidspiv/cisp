/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 5
# Date:             9/23/24
# Description:      Calculate the time scalar to celestial bodies via three
#                   user inputs: A heliocentric origin point, a celestial body,
#                   and a speed in miles per hour. Show sum of time scalars for
#                   all waypoints entered during runtime.
# Sources:          Assignment 3 specifications
#                   planet planets are relative to the current date and were
#                   sourced from NASA 9/9/24, 9028.83 days since J2000 epoch
#******************************************************************************/

#include "valid1.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

struct Planet {
   string name;
   double distance;
};

// utility
string toLowercase(string input);
string formatDouble(double yearsAsDouble);
string formatTimeResult(const string &label, double years);
void expandArray(int *&arr, size_t &currentMax);
void expandArray(Planet *&arr, size_t &currentMax);

// output
void print(const string &output, bool carriageReturn = 1);
void printMenu();
void printHistory(int *&origins, Planet *&planets, int *&velocities,
                  int totalInputs);

// input
int getMenuChoice();
Planet getPlanet();
int getGeocentricOrigin();
int getVelocity();

// business
double calcYears(int geocentricOrigin, double planetDistance, int velocity);
double calcTotalTime(int *origins, Planet *planets, int *velocities);
void addWaypoint(int &origin, Planet &planet, int &velocity);

int main()
{
   const int ADD_WAYPOINT = 1;
   const int HISTORY = 2;
   const int TOTAL = 3;
   const int QUIT = 4;

   int menuChoice = 0;
   int totalInputs = 0;
   size_t currentMax = 10;

   int *origins = nullptr;
   Planet *planets = nullptr;
   int *velocities = nullptr;

   double totalTime = 0.0;
   string resultString = "";

   print("Planet Trip Calculator");

   origins = new int[currentMax];
   planets = new Planet[currentMax];
   velocities = new int[currentMax];

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {

         if (totalInputs == static_cast<int>(currentMax)) {
            expandArray(origins, currentMax);
            expandArray(planets, currentMax);
            expandArray(velocities, currentMax);
         }

         addWaypoint(origins[totalInputs], planets[totalInputs],
                     velocities[totalInputs]);

         totalInputs += 1;
      }
      else if (menuChoice == HISTORY) {

         printHistory(origins, planets, velocities, totalInputs);
      }
      else if (menuChoice == TOTAL) {
         totalTime = calcTotalTime(origins, planets, velocities);
         resultString = formatTimeResult("Total time", totalTime);
         print(resultString);
      }

   } while (menuChoice != QUIT);


   delete[] origins;
   delete[] planets;
   delete[] velocities;
   origins = nullptr;
   planets = nullptr;
   velocities = nullptr;

   print("\nEnd program.");

   return 0;
}


// print() outputs value to console
// Pre: output is a string, carriageReturn is a bool
// Post: output is printed and output stream buffer flushed
void print(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }
   cout << output << flush;
}


// printMenu() shows menu choices that correspond to integer constants
// Pre: none
// Post: menu is printed
void printMenu()
{
   print(R"(
1. Add waypoint
2. History
3. Total
4. Quit)");
}


// getMenuChoice() gets menu option and returns it
// Pre: none
// Post: returns integer between 0 and 3 noninclusive
int getMenuChoice()
{
   int menuOption = 0;

   printMenu();
   menuOption = getInteger("Choose option: ");

   while (menuOption <= 0 || menuOption > 4) {
      print("Must choose number from menu options.");
      printMenu();
      menuOption = getInteger("Choose option: ");
   }

   return menuOption;
}


// toLowercase() will transform any uppercase letter to lowercase
// Pre: input must be string
// Post: input is returned as all lowercase string
string toLowercase(string input)
{
   for (size_t charIndex = 0; charIndex < input.length(); charIndex += 1) {
      input[charIndex] = tolower(input[charIndex]);
   }

   return input;
}


// getPlanet() matches a user input with a list of celestial bodies, returns a
// Planet struct with a name and a distance from earth associated with each
// body
// with each body
// Pre: none
// Post: planetDistance is a Planet struct
Planet getPlanet()
{
   const double MILES_PER_AU = 9.296e+7;

   const double VENUS_DISTANCE_AS_AU = 1.42;
   const double MERCURY_DISTANCE_AS_AU = 1.29;
   const double MARS_DISTANCE_AS_AU = 1.31;
   const double JUPITER_DISTANCE_AS_AU = 4.83;
   const double SATURN_DISTANCE_AS_AU = 8.68;
   const double URANUS_DISTANCE_AS_AU = 19.04;
   const double NEPTUNE_DISTANCE_AS_AU = 28.89;
   const double PLUTO_DISTANCE_AS_AU = 34.56;

   double distance = 0;
   string name = "";

   Planet planet;

   while (distance < 1) {

      name = getString("Enter a planet in our solar system: ");
      name = toLowercase(name);

      if (name.length() > 10) {
         print("Planet must be within our solar system.");
      }
      else if (name.find("mercury") != string::npos) {
         distance = MERCURY_DISTANCE_AS_AU;
      }
      else if (name.find("venus") != string::npos) {
         distance = VENUS_DISTANCE_AS_AU;
      }
      else if (name.find("mars") != string::npos) {
         distance = MARS_DISTANCE_AS_AU;
      }
      else if (name.find("jupiter") != string::npos) {
         distance = JUPITER_DISTANCE_AS_AU;
      }
      else if (name.find("uranus") != string::npos) {
         distance = URANUS_DISTANCE_AS_AU;
      }
      else if (name.find("saturn") != string::npos) {
         distance = SATURN_DISTANCE_AS_AU;
      }
      else if (name.find("pluto") != string::npos) {
         distance = PLUTO_DISTANCE_AS_AU;
      }
      else if (name.find("neptune") != string::npos) {
         distance = NEPTUNE_DISTANCE_AS_AU;
      }
      else {
         print("Planet must be within our solar system.");
      }
   }

   distance = distance * MILES_PER_AU;

   planet.name = name;
   planet.distance = distance;

   return planet;
}


// getGeocentricOrigin() gets a user input is miles from earth and returns
// it
// Pre: none
// Post: geocentricOrigin is an integer in between 0 and 100K
int getGeocentricOrigin()
{
   int geocentricOrigin = getInteger("\nEnter origin (miles from Earth): ");

   while (geocentricOrigin <= 0 || geocentricOrigin >= 100000) {
      print("Must be greater than 0 and less than 100K miles.");
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
      print("Must be greater than 0.");
      velocityAsMph = getInteger("Enter miles per hour: ");
   }

   return velocityAsMph;
}


// calcYears() will calculate the distance between an origin and a planet,
// returns the time it takes to get there
// Pre: milesFromEarth is an int, planetDistance is a Planet struct,
// velocityAsMph is an int
// Post: a positive integer is returned
double calcYears(int geocentricOrigin, double planetDistance, int velocity)
{
   const int HOURS_PER_YEAR = 8760;

   const double distanceInMiles = abs(planetDistance - geocentricOrigin);
   const double years = distanceInMiles / velocity / HOURS_PER_YEAR;

   return years;
}


// expandArray() allocates an identical array except it has [step]
// more slots than the previous array
// Pre: arr is an int pointer reference to a dynamically allocated array,
// size is length of array, step is the length it will be increased by
// Post: pointer argument now references larger array
void expandArray(int *&arr, size_t &currentMax)
{
   const size_t STEP = 10;
   int copyArr[currentMax];

   for (size_t i = 0; i < currentMax; i++) {
      copyArr[i] = arr[i];
   }

   delete[] arr;
   arr = new int[currentMax + STEP];

   for (size_t i = 0; i < currentMax; i++) {
      arr[i] = copyArr[i];
   }

   currentMax += STEP;
}


// overloaded for Planet struct
void expandArray(Planet *&arr, size_t &currentMax)
{
   const size_t step = currentMax;
   Planet copyArr[currentMax];

   for (size_t i = 0; i < currentMax; i++) {
      copyArr[i].name = arr[i].name;
      copyArr[i].distance = arr[i].distance;
   }

   delete[] arr;
   arr = new Planet[currentMax + step];

   for (size_t i = 0; i < currentMax; i++) {
      arr[i].name = copyArr[i].name;
      arr[i].distance = copyArr[i].distance;
   }

   currentMax += step;
}


// addWaypoint() calculates the time scalar to celestial bodies
// Pre: origin is integer reference, distance is double reference, velocity
// is integer reference
// Post: all data arrays are increased by one with correct data and waypoint
// distance is displayed
void addWaypoint(int &origin, Planet &planet, int &velocity)
{
   double years = 0;
   string resultString = "";

   origin = getGeocentricOrigin();
   planet = getPlanet();
   velocity = getVelocity();
   years = calcYears(origin, planet.distance, velocity);
   resultString = formatTimeResult("Travel time", years);

   print(resultString);
}


// calcTotalTime() loops through data arrays and prints out past user inputs and times
void printHistory(int *&origins, Planet *&planets, int *&velocities,
                  int totalInputs)
{
   cout << setw(15) << left << "\nOrigin" << setw(15) << " Planet" << setw(15)
        << " Distance" << setw(20) << " Time";
   cout << setw(15) << left << "\n------" << setw(15) << " ------" << setw(15)
        << " --------" << setw(20) << " ----" << endl;


   for (int i = 0; i < totalInputs; i++) {
      const double years =
          calcYears(origins[i], planets[i].distance, velocities[i]);

      cout << setw(15) << left << to_string(origins[i]) << setw(15)
           << planets[i].name << setw(15) << to_string(velocities[i])
           << setw(20) << to_string(years) << endl;
   }
}


// calcTotalTime() loops through the array and calculates each distance
// value, returning the sum
// Pre: origins is an int pointer, planets is a Planet struct pointer,
// velocities as an int pointer totalTime is a float
// Post: totalTime is double
double calcTotalTime(int *origins, Planet *planets, int *velocities)
{
   int validIndex = 0;
   double years = 0;
   double totalTime = 0;

   while (origins[validIndex] != 0 && planets[validIndex].distance != 0 &&
          velocities[validIndex] != 0) {

      years = calcYears(origins[validIndex], planets[validIndex].distance,
                        velocities[validIndex]);

      totalTime += years;
      validIndex += 1;
   }

   return totalTime;
}


// formatDouble() will convert a double to a comma separated value truncated
// to two decimal places
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


// formatTimeResult() converts time format if needed and display a border
// around the output
// Pre: label and a const string reference, years as a double
// Post: formattedTimeResult is a string;
string formatTimeResult(const string &label, double years)
{
   string formattedTime = label + ": ";
   string resultBorder = "";

   if (years == 0) {
      formattedTime += "None";
   }
   else if (years == 1) {
      formattedTime += "1 year";
   }
   else if (years < 1) {
      formattedTime += to_string(years) + " of a year";
   }
   else if (years > 100000) {
      formattedTime += formatDouble(years / 1000) + " millennia";
   }
   else {
      formattedTime += formatDouble(years) + " years";
   }

   for (size_t i = 0; i < formattedTime.length(); i += 1) {
      resultBorder += "-";
   }

   return "\n" + resultBorder + "\n" + formattedTime + "\n" + resultBorder;
}
