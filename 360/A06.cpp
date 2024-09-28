/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 6
# Date:             9/27/24
# Description:      Calculate the time scalar to celestial bodies via three
#                   user inputs: a celestial body,
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
   double semiMajorAxis_AU;
   double eccentricity;
   double orbitalInclination_Deg;
   double longitudeOfAscendingNode_Deg;
   double longitudeOfPerihelion_Deg;
   double meanAnomaly_Deg;
   double period;
};

struct Cord {
   double x;
   double y;
   double z;
};

struct Input {
   string name;
   string formattedTime;
   float milesPerHour;
};

struct Result {
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
   for (size_t charIndex = 0; charIndex < input.length(); charIndex += 1) {
      input[charIndex] = tolower(input[charIndex]);
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
   } while (planetIndex > 0);

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

void calcYears(const Input &input, Result &result)
{
   const int HOURS_PER_YEAR = 8760;
   result.timeAsYears =
       result.geocentricDistance / input.milesPerHour / HOURS_PER_YEAR;
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

void printHistory(Input *&inputs, Result *&results, size_t numInputs)
{
   cout << endl
        << " Planet" << setw(10) << " Time" << setw(20) << " Distance" << endl;
   cout << " ------" << setw(10) << " ----" << setw(20) << " --------" << endl;

   for (size_t i = 0; i < numInputs; i++) {
      calcYears(inputs[i], results[i]);

      cout << setw(10) << inputs[i].name << setw(10)
           << to_string(inputs[i].milesPerHour) << setw(20)
           << to_string(results[i].timeAsYears) << endl;
   }
}

double calcTotalTime(Input *&inputs, Result *&results)
{
   int validIndex = 0;
   double totalTime = 0;

   while (inputs[validIndex].formattedTime != "" &&
          inputs[validIndex].milesPerHour != 0) {

      calcYears(inputs[validIndex], results[validIndex]);

      totalTime += results[validIndex].timeAsYears;
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

Planet *populatePlanets()
{
   Planet mercury;
   mercury.name = "mercury";
   mercury.semiMajorAxis_AU = 0.38709893;
   mercury.eccentricity = 0.20563069;
   mercury.orbitalInclination_Deg = 7.00487;
   mercury.longitudeOfAscendingNode_Deg = 48.33167;
   mercury.longitudeOfPerihelion_Deg = 77.45645;
   mercury.meanAnomaly_Deg = 174.796;
   mercury.period = 87.969;

   Planet venus;
   venus.name = "venus";
   venus.semiMajorAxis_AU = 0.72333199;
   venus.eccentricity = 0.00677323;
   venus.orbitalInclination_Deg = 3.39471;
   venus.longitudeOfAscendingNode_Deg = 76.68069;
   venus.longitudeOfPerihelion_Deg = 131.53298;
   venus.meanAnomaly_Deg = 50.45;
   venus.period = 224.7008;

   Planet earth;
   earth.name = "earth";
   earth.semiMajorAxis_AU = 1.00000011;
   earth.eccentricity = 0.01671022;
   earth.orbitalInclination_Deg = 0.00005;
   earth.longitudeOfAscendingNode_Deg = -11.26064;
   earth.longitudeOfPerihelion_Deg = 102.94719;
   earth.meanAnomaly_Deg = 357.51716;
   earth.period = 365.259636;

   Planet mars;
   mars.name = "mars";
   mars.semiMajorAxis_AU = 1.52366231;
   mars.eccentricity = 0.09341233;
   mars.orbitalInclination_Deg = 1.85061;
   mars.longitudeOfAscendingNode_Deg = 49.57854;
   mars.longitudeOfPerihelion_Deg = 336.04084;
   mars.meanAnomaly_Deg = 19.387;
   mars.period = 686.9957;

   Planet jupiter;
   jupiter.name = "jupiter";
   jupiter.semiMajorAxis_AU = 5.20336301;
   jupiter.eccentricity = 0.04839266;
   jupiter.orbitalInclination_Deg = 1.30530;
   jupiter.longitudeOfAscendingNode_Deg = 100.55615;
   jupiter.longitudeOfPerihelion_Deg = 14.75385;
   jupiter.meanAnomaly_Deg = 20.020;
   jupiter.period = 11.862;

   Planet saturn;
   saturn.name = "saturn";
   saturn.semiMajorAxis_AU = 9.53707032;
   saturn.eccentricity = 0.05415060;
   saturn.orbitalInclination_Deg = 2.48446;
   saturn.longitudeOfAscendingNode_Deg = 113.71504;
   saturn.longitudeOfPerihelion_Deg = 92.43194;
   saturn.meanAnomaly_Deg = 317.020;
   saturn.period = 29.4475;

   Planet uranus;
   uranus.name = "uranus";
   uranus.semiMajorAxis_AU = 19.19126393;
   uranus.eccentricity = 0.04716771;
   uranus.orbitalInclination_Deg = 0.76986;
   uranus.longitudeOfAscendingNode_Deg = 74.22988;
   uranus.longitudeOfPerihelion_Deg = 170.96424;
   uranus.meanAnomaly_Deg = 142.238600;
   uranus.period = 84.011;

   Planet neptune;
   neptune.name = "neptune";
   neptune.semiMajorAxis_AU = 30.06896348;
   neptune.eccentricity = 0.00858587;
   neptune.orbitalInclination_Deg = 1.76917;
   neptune.longitudeOfAscendingNode_Deg = 131.72169;
   neptune.longitudeOfPerihelion_Deg = 44.97135;
   neptune.meanAnomaly_Deg = 259.883;
   neptune.period = 164.79;

   Planet *planets = new Planet[8]{mercury, venus,  earth,  mars,
                                   jupiter, saturn, uranus, neptune};

   return planets;
}

// convert to scalar between 0 and 360 inclusive.
double normalizeAngle(double scalar)
{
   double mod = remainder(scalar, 360);
   if (mod < 0) {
      mod += 360;
   }
   return mod;
}

double getDayNum()
{
   const int year = 2024;
   const int month = 9;
   const int day = 9;
   const double universalTime = 13.10;

   // intentional integer division
   double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                    3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                    275 * month / 9 + day - 730515;

   totDays -= universalTime / 24.0;

   return totDays;
}

double toRadians(double degrees)
{
   return degrees * (M_PI / 180.0);
}

// double radiusVectors[] = {0.0, 0.0, 0.0};

Cord getHeliocentricCords(const Planet &body, int dayNum)
{
   const double diurnalMotion = 360 / body.period;
   const double normalizedMeanAnomaly_Deg =
       normalizeAngle(body.meanAnomaly_Deg + diurnalMotion * dayNum);

   const double a = body.semiMajorAxis_AU;
   const double e = body.eccentricity;
   const double M = toRadians(normalizedMeanAnomaly_Deg);
   const double o = toRadians(body.longitudeOfAscendingNode_Deg);
   const double p = toRadians(body.longitudeOfPerihelion_Deg);
   const double i = toRadians(body.orbitalInclination_Deg);

   // Initial approximation of Eccentric Anomaly (E) using Kepler's equation
   double E = M + e * sin(M) * (1 + e * cos(M));
   double delta;

   // Iteratively solve for E using Newton-Raphson method
   do {
      const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
      delta = abs(E1 - E);
      E = E1;
   } while (delta >= 0.0001);

   // Compute the position in orbit plane
   const double xv = a * (cos(E) - e);
   const double yv = a * (sqrt(1.0 - e * e) * sin(E));

   // Calculate trueAnomaly (v) and radius vector (r)
   const double v = atan2(yv, xv);
   const double r = sqrt(xv * xv + yv * yv);

   // Convert to 3D coordinates in space
   const double x =
       r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
   const double y =
       r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
   const double z = r * (sin(v + p - o) * sin(i));

   cout << "From sun (AU) " << body.name << ": " << r << endl;

   return {x, y, z};
}

void addWaypoint(const Planet *&planets, const Input &input, Result &result)
{
   double years = 0;
   string totalTimeAsString = "";

   const size_t planetIndex = getPlanetIndex();
   const int velocity = getVelocity();

   const double dayNum = getDayNum();

   const Planet planet = planets[planetIndex];
   const Cord heliocentricCord = getHeliocentricCords(planet, dayNum);
   const Cord heliocentricCordEarth = getHeliocentricCords(planets[2], dayNum);

   const double gX = heliocentricCordEarth.x - heliocentricCord.x;
   const double gY = heliocentricCordEarth.y - heliocentricCord.y;
   const double gZ = heliocentricCordEarth.z - heliocentricCord.z;

   result.geocentricDistance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

   calcYears(input, result);
   totalTimeAsString = formatTimeResult("Travel time", years);

   print(totalTimeAsString);
}

int main()
{
   int menuChoice = 0;
   size_t indexInput = 0;
   size_t indexMax = 10;

   const Planet *planets = populatePlanets();
   Input *inputs = new Input[indexMax];
   Result *results = new Result[indexMax];

   print("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice();

      if (menuChoice == ADD_WAYPOINT) {

         if (indexInput == indexMax) {
            expandArray<Input>(inputs, indexMax);
            expandArray<Result>(results, indexMax);
         }

         addWaypoint(planets, inputs[indexInput], results[indexInput]);

         indexInput += 1;
      }
      else if (menuChoice == HISTORY) {

         printHistory(inputs, results, indexInput);
      }
      else if (menuChoice == TOTAL) {
         double totalTime = calcTotalTime(inputs, results);
         string totalTimeAsString = formatTimeResult("Total time", totalTime);

         print(totalTimeAsString);
      }

   } while (menuChoice != QUIT);

   delete[] planets;
   delete[] results;
   planets = nullptr;
   results = nullptr;

   print("\nEnd program.");

   return 0;
}
