/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 6
# Date:             9/27/24
# Description:      Calculate the time scalar to celestial bodies via three
#                   user inputs: a celestial planet,
#                   and a speed in miles per hour. Show sum of time scalars for
#                   all waypoints entered during runtime.
# Sources:          Assignment 3 specifications
#                   planet planets are relative to the current date and were
#                   sourced from NASA 9/9/24, 9028.83 days since J2000 epoch
#******************************************************************************/

#include "valid.h"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

struct Planet {
   string name;
   double semiMajorAxis;
   double eccentricity;
   double orbitalInclination;
   double longitudeOfAscendingNode;
   double longitudeOfPerihelion;
   double meanAnomaly;
   double orbitalPeriod;
};

struct Cord {
   double x;
   double y;
   double z;
};

struct Waypoint {
   string name;
   double velocity;
   double geocentricDistance;
   double timeAsYears;
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
   for (char &character : input) {
      character = tolower(character);
   }

   return input;
}

size_t getPlanetIndex()
{
   string name = "";
   double planetIndex = -1;
   cout << endl;

   do {
      name = getString("Enter a planet in our solar system: ");
      name = toLowercase(name);

      if (name.length() > 10) {
         print("Planet must be within our solar system.");
      }
      else if (name.find("mercury") != string::npos) {
         planetIndex = 0;
      }
      else if (name.find("venus") != string::npos) {
         planetIndex = 1;
      }
      else if (name.find("mars") != string::npos) {
         planetIndex = 3;
      }
      else if (name.find("jupiter") != string::npos) {
         planetIndex = 4;
      }
      else if (name.find("uranus") != string::npos) {
         planetIndex = 5;
      }
      else if (name.find("saturn") != string::npos) {
         planetIndex = 6;
      }
      else if (name.find("neptune") != string::npos) {
         planetIndex = 7;
      }
      else {
         print("Planet must be within our solar system.");
      }
   } while (planetIndex == -1);

   return planetIndex;
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

double calcYears(double distanceAsAU, double velocityAsMph)
{
   const int HOURS_PER_YEAR = 8760;
   const double MILES_PER_AU = 9.296e+7;

   const double distanceAsMiles = distanceAsAU * MILES_PER_AU;

   return distanceAsMiles / velocityAsMph / HOURS_PER_YEAR;
}

template <typename T> void expandArray(T *&arr, size_t &indexMax)
{
   const size_t step = 10;
   T copyArr[indexMax];

   for (size_t i = 0; i < indexMax; i++) {
      copyArr[i] = arr[i];
   }

   delete[] arr;
   arr = new T[indexMax + step];

   for (size_t i = 0; i < indexMax; i++) {
      arr[i] = copyArr[i];
   }

   indexMax += step;
}

void printHistory(const Waypoint *waypoints, size_t numInputs)
{
   cout << endl
        << setw(8) << left << "Planet" << setw(15) << "Distance (AU)"
        << setw(10) << "Time (yrs)" << endl;
   cout << setw(8) << left << "------" << setw(15) << "-------------"
        << setw(10) << "----------" << endl;

   for (size_t i = 0; i < numInputs; i++) {

      cout << setw(8) << left << waypoints[i].name << setw(15)
           << waypoints[i].geocentricDistance << setw(10)
           << waypoints[i].timeAsYears << endl;
   }
}

double calcTotalTime(Waypoint *&waypoints)
{
   int validIndex = 0;
   double totalTime = 0;

   while (waypoints[validIndex].name != "" &&
          waypoints[validIndex].velocity != 0) {

      totalTime += waypoints[validIndex].timeAsYears;
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

   resultBorder = string(formattedTime.length(), '-');

   return "\n" + resultBorder + "\n" + formattedTime + "\n" + resultBorder;
}

Planet *populatePlanets()
{
   Planet mercury;
   mercury.name = "mercury";
   mercury.semiMajorAxis = 0.38709893;
   mercury.eccentricity = 0.20563069;
   mercury.orbitalInclination = 7.00487;
   mercury.longitudeOfAscendingNode = 48.33167;
   mercury.longitudeOfPerihelion = 77.45645;
   mercury.meanAnomaly = 174.796;
   mercury.orbitalPeriod = 87.969;

   Planet venus;
   venus.name = "venus";
   venus.semiMajorAxis = 0.72333199;
   venus.eccentricity = 0.00677323;
   venus.orbitalInclination = 3.39471;
   venus.longitudeOfAscendingNode = 76.68069;
   venus.longitudeOfPerihelion = 131.53298;
   venus.meanAnomaly = 50.45;
   venus.orbitalPeriod = 224.7008;

   Planet earth;
   earth.name = "earth";
   earth.semiMajorAxis = 1.00000011;
   earth.eccentricity = 0.01671022;
   earth.orbitalInclination = 0.00005;
   earth.longitudeOfAscendingNode = -11.26064;
   earth.longitudeOfPerihelion = 102.94719;
   earth.meanAnomaly = 357.51716;
   earth.orbitalPeriod = 365.259636;

   Planet mars;
   mars.name = "mars";
   mars.semiMajorAxis = 1.52366231;
   mars.eccentricity = 0.09341233;
   mars.orbitalInclination = 1.85061;
   mars.longitudeOfAscendingNode = 49.57854;
   mars.longitudeOfPerihelion = 336.04084;
   mars.meanAnomaly = 19.387;
   mars.orbitalPeriod = 686.9957;

   Planet jupiter;
   jupiter.name = "jupiter";
   jupiter.semiMajorAxis = 5.20336301;
   jupiter.eccentricity = 0.04839266;
   jupiter.orbitalInclination = 1.30530;
   jupiter.longitudeOfAscendingNode = 100.55615;
   jupiter.longitudeOfPerihelion = 14.75385;
   jupiter.meanAnomaly = 20.020;
   jupiter.orbitalPeriod = 11.862;

   Planet saturn;
   saturn.name = "saturn";
   saturn.semiMajorAxis = 9.53707032;
   saturn.eccentricity = 0.05415060;
   saturn.orbitalInclination = 2.48446;
   saturn.longitudeOfAscendingNode = 113.71504;
   saturn.longitudeOfPerihelion = 92.43194;
   saturn.meanAnomaly = 317.020;
   saturn.orbitalPeriod = 29.4475;

   Planet uranus;
   uranus.name = "uranus";
   uranus.semiMajorAxis = 19.19126393;
   uranus.eccentricity = 0.04716771;
   uranus.orbitalInclination = 0.76986;
   uranus.longitudeOfAscendingNode = 74.22988;
   uranus.longitudeOfPerihelion = 170.96424;
   uranus.meanAnomaly = 142.238600;
   uranus.orbitalPeriod = 84.011;

   Planet neptune;
   neptune.name = "neptune";
   neptune.semiMajorAxis = 30.06896348;
   neptune.eccentricity = 0.00858587;
   neptune.orbitalInclination = 1.76917;
   neptune.longitudeOfAscendingNode = 131.72169;
   neptune.longitudeOfPerihelion = 44.97135;
   neptune.meanAnomaly = 259.883;
   neptune.orbitalPeriod = 164.79;

   Planet *planets = new Planet[8]{mercury, venus,  earth,  mars,
                                   jupiter, saturn, uranus, neptune};

   return planets;
}

// ensure the result is always within the standard circle range
double normalizeDegrees(double scalar)
{
   double mod = remainder(scalar, 360);
   if (mod < 0) {
      mod += 360;
   }
   return mod;
}

double calcDaysSinceEpoch()
{
   const int year = 2024;
   const int month = 9;
   const int day = 28;

   // intentional integer division
   double totalDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                      3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                      275 * month / 9 + day - 730515;

   return totalDays;
}

double toRadians(double degrees)
{
   return degrees * (M_PI / 180.0);
}

Cord getHeliocentricCords(const Planet &planet, int daysSinceEpoch)
{
   const double diurnalMotion = 360.0 / planet.orbitalPeriod;

   const double normalizedMeanAnomaly =
       normalizeDegrees(planet.meanAnomaly + diurnalMotion * daysSinceEpoch);

   const double a = planet.semiMajorAxis;
   const double e = planet.eccentricity;
   const double M = toRadians(normalizedMeanAnomaly);
   const double o = toRadians(planet.longitudeOfAscendingNode);
   const double p = toRadians(planet.longitudeOfPerihelion);
   const double i = toRadians(planet.orbitalInclination);

   // Initial approximation of Eccentric Anomaly (E) using Kepler's equation.
   double E = M + e * sin(M) * (1 + e * cos(M));
   double delta;

   // Refine E with Newton-Raphson method. This method will not work with
   // parabolic or near-parabolic orbits (when the eccentricity is close to 1).
   do {
      const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
      delta = abs(E1 - E);
      E = E1;
   } while (delta >= 0.0001);

   // compute position in orbital plane
   const double xv = a * (cos(E) - e);
   const double yv = a * (sqrt(1.0 - e * e) * sin(E));

   // The True Anomaly (v) is the final angle we need to define the position
   // of a satellite in orbit, the other two being Eccentric Anomaly (E) and
   // Mean Anomaly (M).
   const double v = atan2(yv, xv);

   // The radius vector (r) is the distance of the satellite to the focal point
   // of the ellipse, in this case the sun.
   const double r = sqrt(xv * xv + yv * yv);

   // compute heliocentric cartesian coordinates
   const double x =
       r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
   const double y =
       r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
   const double z = r * (sin(v + p - o) * sin(i));

   return {x, y, z};
}

Waypoint createWaypoint(const Planet *&planets)
{
   string totalTimeAsString = "";

   const size_t planetIndex = getPlanetIndex();
   const Planet planet = planets[planetIndex];

   const double daysSinceEpoch = calcDaysSinceEpoch();

   const Cord heliocentricCord = getHeliocentricCords(planet, daysSinceEpoch);
   const Cord heliocentricCordEarth =
       getHeliocentricCords(planets[2], daysSinceEpoch);

   const double gX = heliocentricCordEarth.x - heliocentricCord.x;
   const double gY = heliocentricCordEarth.y - heliocentricCord.y;
   const double gZ = heliocentricCordEarth.z - heliocentricCord.z;

   const double velocity = getVelocity();
   const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

   const double years = calcYears(distance, velocity);

   totalTimeAsString = formatTimeResult("Travel time", years);
   print(totalTimeAsString);

   return {planet.name, velocity, distance, years};
}

int main()
{
   int menuChoice = 0;
   size_t indexInput = 0;
   size_t indexMax = 10;

   const Planet *planets = populatePlanets();
   Waypoint *waypoints = new Waypoint[indexMax];

   print("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {

         if (indexInput == indexMax) {
            expandArray<Waypoint>(waypoints, indexMax);
         }

         waypoints[indexInput] = createWaypoint(planets);
         indexInput += 1;
      }
      else if (menuChoice == HISTORY) {

         printHistory(waypoints, indexInput);
      }
      else if (menuChoice == TOTAL) {
         double totalTime = calcTotalTime(waypoints);
         string totalTimeAsString = formatTimeResult("Total time", totalTime);

         print(totalTimeAsString);
      }

   } while (menuChoice != QUIT);

   delete[] planets;
   delete[] waypoints;
   planets = nullptr;
   waypoints = nullptr;

   print("\nEnd program.");

   return 0;
}
