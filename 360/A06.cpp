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

#include "valid.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

struct Planet {
   string name;
   double distance;
};

const int ADD_WAYPOINT = 1;
const int HISTORY = 2;
const int TOTAL = 3;
const int QUIT = 4;

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
   print(R"(
1. Add waypoint
2. History
3. Total
4. Quit)");
}

int getMenuChoice()
{
   int menuOption = 0;

   printMenu();
   menuOption = getInteger("Choose option: ");

   while (menuOption <= 0 || menuOption > QUIT) {
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

double calcYears(int geocentricOrigin, double planetDistance, int velocity)
{
   const int HOURS_PER_YEAR = 8760;

   const double distanceInMiles = abs(planetDistance - geocentricOrigin);
   const double years = distanceInMiles / velocity / HOURS_PER_YEAR;

   return years;
}

template <typename T> void expandArray(T *&arr, size_t &currentMax)
{
   const size_t step = 10;
   T copyArr[currentMax];

   for (size_t i = 0; i < currentMax; i++) {
      copyArr[i] = arr[i];
   }

   delete[] arr;
   arr = new T[currentMax + step];

   for (size_t i = 0; i < currentMax; i++) {
      arr[i] = copyArr[i];
   }

   currentMax += step;
}

void printHistory(int *&origins, Planet *&planets, int *&velocities,
                  int totalInputs)
{
   cout << setw(10) << left << "\nOrigin" << setw(10) << " Planet" << setw(10)
        << " MPH" << setw(20) << " Time";
   cout << setw(10) << left << "\n------" << setw(10) << " ------" << setw(10)
        << " ---" << setw(20) << " ----" << endl;

   for (int i = 0; i < totalInputs; i++) {
      const double years =
          calcYears(origins[i], planets[i].distance, velocities[i]);

      cout << setw(10) << left << to_string(origins[i]) << setw(10)
           << planets[i].name << setw(10) << to_string(velocities[i])
           << setw(20) << to_string(years) << endl;
   }
}

double calcTotalTime(int *origins, Planet *planets, int *velocities)
{
   int validIndex = 0;
   double totalTime = 0;

   while (origins[validIndex] != 0 && planets[validIndex].distance != 0 &&
          velocities[validIndex] != 0) {

      double years =
          calcYears(origins[validIndex], planets[validIndex].distance,
                    velocities[validIndex]);

      totalTime += years;
      validIndex += 1;
   }

   return totalTime;
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

void addWaypoint(int &origin, Planet &planet, int &velocity)
{
   double years = 0;
   string totalTimeAsString = "";

   origin = getGeocentricOrigin();
   planet = getPlanet();
   velocity = getVelocity();

   years = calcYears(origin, planet.distance, velocity);
   totalTimeAsString = formatTimeResult("Travel time", years);

   print(totalTimeAsString);
}

int main()
{
   int menuChoice = 0;
   size_t totalInputs = 0;
   size_t currentMax = 10;

   int *origins = new int[currentMax];
   Planet *planets = new Planet[currentMax];
   int *velocities = new int[currentMax];


   print("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {

         if (totalInputs == currentMax) {
            expandArray<int>(origins, currentMax);
            expandArray<Planet>(planets, currentMax);
            expandArray<int>(velocities, currentMax);
         }

         addWaypoint(origins[totalInputs], planets[totalInputs],
                     velocities[totalInputs]);

         totalInputs += 1;
      }
      else if (menuChoice == HISTORY) {

         printHistory(origins, planets, velocities, totalInputs);
      }
      else if (menuChoice == TOTAL) {
         double totalTime = calcTotalTime(origins, planets, velocities);
         string totalTimeAsString = formatTimeResult("Total time", totalTime);

         print(totalTimeAsString);
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
