/******************************************************************************
# Author:           David Spivack
# Assignment:       Assignment 6
# Date:             9/29/24
# Description:      Calculate travel time according to three user inputs: A
#                   date, a planet in our solar system, and an average
#                   velocity. Menu options include displaying the sum of time
#                   scalars for all waypoints entered during runtime and
#                   showing the input history.
# Sources:          Assignment 6 specifications
#
#                   Computing the position of the planets in the sky:
#                   https://stjarnhimlen.se/comp/ppcomp.html
#                   http://www.stargazing.net/kepler/ellipse.html
#                   https://jtauber.github.io/orbits/
#
#                   Orbital elements data:
#                   https://nssdc.gsfc.nasa.gov/planetary/factsheet/
#
#                   Verifying results:
#                   https://www.heavens-above.com/planets.aspx
#                   https://marsclock.com/
#******************************************************************************/

#include "valid.h"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

// The Planet struct encompasses orbital elements; the information needed to
// define a position of a planet in orbit. These parameters specify the
// angle of orbit in three dimensions and the position of the planet along that
// orbit. Measurements are relative to the J2000 epoch (Jan 1, 2000) and/or
// to the plane of the ecliptic (the imaginary plane containing the Earth's
// orbit around the Sun) where specified.

struct Planet {
   string name;
   double semiMajorAxis;
   double eccentricity;
   double orbitalInclination;
   double longitudeOfAscendingNode;
   double longitudeOfPerihelion;
   double meanAnomaly;
   double period;
};

// Semi-major Axis: [units: AU] half of the length of the long axis of the
// ellipse (where AU is an astronomical unit, the mean distance between the
// Earth and the Sun)

// Eccentricity: [units: N/A] ratio between the focal distance (which is
// relative to J2000) and the semi-major axis

// Orbital Inclination: [units: degrees] The tilt of the planet's orbit around
// the sun relative to plane of the ecliptic

// Longitude of the Ascending Node: [units: degrees] angular position on plane
// of the ecliptic relative to J2000

// Longitude of Perihelion: [units: degrees] the point in orbit closest to sun
// relative to the plane of the ecliptic. All three anomalies (calculated later
// on) are zero when the planet is in perihelion. The longitude of the
// ascending node subtracted from this angle is known as the argument of
// the periapsis

// Mean Anomaly: [units: degrees] the fraction of the period that has elapsed
// since the planet passed through the perihelion relative to J2000

// Period: [units: days] refers to the time around the sun

// Visualization in 2D - https://shorturl.at/ZL1Cr. Where V = trueAnomaly,
// E = eccentricAnomaly, B = perihelion, P = planet, and the distance between A
// and B represents the major axis. The orbital inclination exists in the
// dimension not shown. NOTE: These variable letters reference the link and do
// not align with the rest of the program.

// Visualization in 3D - https://shorturl.at/Cvd1K. The plane of reference
// being the orbital plane of Earth around the sun. The reference direction is
// referred to as "the first point of Aries". Much like how an epoch is defined
// on the timeline, it is an arbitrary reference point. It was defined over
// 2000 years ago by Hipparchus, considered the founder of trigonometry.

// With these measurements, we can account the for position of a planet in
// space at given a point in time, accurate within a few centuries of the
// reference epoch.

// There are two main categories of planetary movement NOT accounted for which
// reduces our accuracy significantly:
// 1) nutation / precession: changes in the planet's axis of rotation
// 2) perturbations: forces other than the sun acting on the planet (other
// satellites). This especially effects the Jovian planets: Saturn, Jupiter,
// Uranus, and Neptune.

// In addition, we simplify the problem so that all planets "freeze" during
// travel time. Accounting for the movement of planets during travel time is
// currently beyond the scope of this project.

struct Cord {
   double x;
   double y;
   double z;
};

struct Date {
   int month;
   int day;
   int year;
};

struct Waypoint {
   Date date;
   string planetName;
   double velocityAsMph;
   double geocentricDistance;
   double timeAsYears;
};

// Utilities
string toLowercase(string input);
template <typename T> void expandArray(T *&arr, size_t &maxInputs);
double normalizeDegrees(double scalar);
double toRadians(double degrees);
int calcDaysSinceEpoch(const Date &date);
string formatDouble(double yearsAsDouble);
string formatTimeResult(const string &label, double years);

// I/O
void print(const string &output, bool carriageReturn = 1);
void printMenu();
void printHistory(const Waypoint *waypoints, size_t numInputs);
void printTotal(Waypoint *&waypoints);
int getMenuChoice(int maxChoice);
Date getDate();
int getPlanetIndex();
int getVelocityAsMph();

// Business
Planet *populatePlanets();
double calcYears(double distanceAsAU, double velocityAsMph);
double calcTotalTime(Waypoint *&waypoints);
double calcEccentricAnomaly(double eccentricity, double normalizedMeanAnomaly);
Cord calcHeliocentricCord(const Planet &planet, int daysSinceEpoch);
Waypoint createWaypoint(const Planet *&planets);

int main()
{
   const int ADD_WAYPOINT = 1;
   const int HISTORY = 2;
   const int TOTAL = 3;
   const int QUIT = 4;

   int menuChoice = 0;
   size_t numInputs = 0;
   size_t maxInputs = 10;

   const Planet *planets = populatePlanets();
   Waypoint *waypoints = new Waypoint[maxInputs];

   print("Planet Trip Calculator");

   do {

      menuChoice = getMenuChoice(QUIT);

      if (menuChoice == ADD_WAYPOINT) {

         if (numInputs == maxInputs) {
            expandArray<Waypoint>(waypoints, maxInputs);
         }

         waypoints[numInputs] = createWaypoint(planets);
         numInputs += 1;
      }
      else if (menuChoice == HISTORY) {

         printHistory(waypoints, numInputs);
      }
      else if (menuChoice == TOTAL) {
         printTotal(waypoints);
      }

   } while (menuChoice != QUIT);

   delete[] planets;
   delete[] waypoints;
   planets = nullptr;
   waypoints = nullptr;

   print("\nEnd program.");

   return 0;
}


// toLowercase() will transform any uppercase letter to lowercase
// Pre: input must be string
// Post: input is returned as an all lowercase string
string toLowercase(string input)
{
   for (char &character : input) {
      character = tolower(character);
   }

   return input;
}


// expandArray() allocates a larger but otherwise identical array
// Pre: arr is a reference int pointer to a dynamically allocated array, size
// is length of array
// Post: pointer argument now references larger array
template <typename T> void expandArray(T *&arr, size_t &maxInputs)
{
   const size_t step = 5;

   T *copyArr = new T[maxInputs + step];

   for (size_t i = 0; i < maxInputs; i++) {
      copyArr[i] = arr[i];
   }

   delete[] arr;
   arr = copyArr;

   maxInputs += step;
}


// normalizeDegrees() ensures degrees is within the standard circle range
// Pre: degrees is double
// Post: normalizedDegrees is double between 0 and 360
double normalizeDegrees(double degrees)
{
   double normalizedDegrees = remainder(degrees, 360);

   if (normalizedDegrees < 0) {
      normalizedDegrees += 360;
   }

   return normalizedDegrees;
}


// toRadians() converts degrees to radians
// Pre: degrees is double between 0 and 360
// Post: radians should be in between 0 and 2π
double toRadians(double degrees)
{
   return degrees * (M_PI / 180.0);
}


// calcDaysSinceEpoch() finds difference in days between date argument and the
// J2000 epoch
// Pre: date is reference Date struct
// Post: total days is integer
int calcDaysSinceEpoch(const Date &date)
{
   const int month = date.month;
   const int day = date.day;
   const int year = date.year;

   // intentional integer division
   int totalDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;

   return totalDays;
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
// Post: formattedTimeResult is a string
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

   // border should be same length as result
   resultBorder = string(formattedTime.length(), '-');

   return "\n" + resultBorder + "\n" + formattedTime + "\n" + resultBorder;
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


// printHistory() loops through data arrays and prints out past inputs and
// results
// Pre: waypoints struct array is populated, numInputs indicates size
// Post: input and result history is displayed to the console
void printHistory(const Waypoint *waypoints, size_t numInputs)
{
   cout << endl
        << setw(11) << left << "Date" << setw(9) << "Planet" << setw(13)
        << "MPH" << setw(9) << "AU" << "Years" << endl;
   cout << setw(11) << left << "----" << setw(9) << "------" << setw(13)
        << "---" << setw(9) << "--" << "-----" << endl;

   for (size_t i = 0; i < numInputs; i++) {

      const string month = to_string(waypoints[i].date.month);
      const string day = to_string(waypoints[i].date.day);
      const string year = to_string(waypoints[i].date.year);

      const string date = month + '/' + day + '/' + year;

      cout << setw(11) << left << date << setw(9) << waypoints[i].planetName
           << setw(13) << waypoints[i].velocityAsMph << setw(9)
           << waypoints[i].geocentricDistance << waypoints[i].timeAsYears
           << endl;
   }
}


// printTotal() formats total and displays to console
// Pre: waypoints struct is populated
// Post: time is displayed to the console
void printTotal(Waypoint *&waypoints)
{
   double totalTime = calcTotalTime(waypoints);
   string totalTimeAsString = formatTimeResult("Total time", totalTime);

   print(totalTimeAsString);
}


// getMenuChoice() gets menu option and returns it
// Pre: none
// Post: returns integer between 0 and 3 noninclusive
int getMenuChoice(int maxChoice)
{
   int menuOption = 0;

   printMenu();
   menuOption = getInteger("Choose option: ");

   while (menuOption <= 0 || menuOption > maxChoice) {
      print("Must choose number from menu options.");
      printMenu();
      menuOption = getInteger("Choose option: ");
   }

   return menuOption;
}


// getDate() gets date string from user input, more lenient than prompt
// suggests
// Pre: will accept three numbers separated by exactly two "/"
// characters
// Post: populated date Struct is returned
Date getDate()
{
   int month = 0;
   int day = 0;
   int year = 0;

   bool isFormatted = false;

   cout << endl;

   do {
      int deliminatorCount = 0;
      string numAsString = "0";

      const string date = getString("Enter a date (MM/DD/YYYY): ");

      for (const char character : date) {

         if (character == '/') {
            deliminatorCount += 1;
            if (deliminatorCount == 1) {
               month = stoi(numAsString);
               numAsString = "0";
            }
            else if (deliminatorCount == 2) {
               day = stoi(numAsString);
               numAsString = "0";
            }
         }
         else {
            numAsString += character;
         }
      }

      year = stoi(numAsString);

      if (deliminatorCount == 2 && month > 0 && month <= 12 && day > 0 &&
          day <= 31) {
         isFormatted = true;
      }
      else {
         print("Date formatted incorrectly, try again");
      }

   } while (!isFormatted);

   return {month, day, year};
}


// getPlanet() matches a user input with a list of celestial bodies
// Pre: none
// Post: planetIndex is a valid Planet struct
int getPlanetIndex()
{
   int planetIndex = -1;

   do {
      string name = getString("Enter a planet in our solar system: ");
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


// getVelocityAsMph() gets a user input is miles per hour and returns it
// Pre: none
// Post: velocityAsMph is an integer greater than 0
int getVelocityAsMph()
{
   int velocityAsMph = getInteger("Enter miles per hour: ");

   while (velocityAsMph <= 0) {
      print("Must be greater than 0.");
      velocityAsMph = getInteger("Enter miles per hour: ");
   }

   return velocityAsMph;
}


// populatePlanets() populates a dynamically allocated array with Planet
// structs and returns the owner pointer
// Pre: none
// Post: planets is an array of Planet structs
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
   mercury.period = 87.969;

   Planet venus;
   venus.name = "venus";
   venus.semiMajorAxis = 0.72333199;
   venus.eccentricity = 0.00677323;
   venus.orbitalInclination = 3.39471;
   venus.longitudeOfAscendingNode = 76.68069;
   venus.longitudeOfPerihelion = 131.53298;
   venus.meanAnomaly = 50.45;
   venus.period = 224.7008;

   Planet earth;
   earth.name = "earth";
   earth.semiMajorAxis = 1.00000011;
   earth.eccentricity = 0.01671022;
   earth.orbitalInclination = 0.00005;
   earth.longitudeOfAscendingNode = -11.26064;
   earth.longitudeOfPerihelion = 102.94719;
   earth.meanAnomaly = 357.51716;
   earth.period = 365.259636;

   Planet mars;
   mars.name = "mars";
   mars.semiMajorAxis = 1.52366231;
   mars.eccentricity = 0.09341233;
   mars.orbitalInclination = 1.85061;
   mars.longitudeOfAscendingNode = 49.57854;
   mars.longitudeOfPerihelion = 336.04084;
   mars.meanAnomaly = 19.387;
   mars.period = 686.9957;

   Planet jupiter;
   jupiter.name = "jupiter";
   jupiter.semiMajorAxis = 5.20336301;
   jupiter.eccentricity = 0.04839266;
   jupiter.orbitalInclination = 1.30530;
   jupiter.longitudeOfAscendingNode = 100.55615;
   jupiter.longitudeOfPerihelion = 14.75385;
   jupiter.meanAnomaly = 20.020;
   jupiter.period = 11.862;

   Planet saturn;
   saturn.name = "saturn";
   saturn.semiMajorAxis = 9.53707032;
   saturn.eccentricity = 0.05415060;
   saturn.orbitalInclination = 2.48446;
   saturn.longitudeOfAscendingNode = 113.71504;
   saturn.longitudeOfPerihelion = 92.43194;
   saturn.meanAnomaly = 317.020;
   saturn.period = 29.4475;

   Planet uranus;
   uranus.name = "uranus";
   uranus.semiMajorAxis = 19.19126393;
   uranus.eccentricity = 0.04716771;
   uranus.orbitalInclination = 0.76986;
   uranus.longitudeOfAscendingNode = 74.22988;
   uranus.longitudeOfPerihelion = 170.96424;
   uranus.meanAnomaly = 142.238600;
   uranus.period = 84.011;

   Planet neptune;
   neptune.name = "neptune";
   neptune.semiMajorAxis = 30.06896348;
   neptune.eccentricity = 0.00858587;
   neptune.orbitalInclination = 1.76917;
   neptune.longitudeOfAscendingNode = 131.72169;
   neptune.longitudeOfPerihelion = 44.97135;
   neptune.meanAnomaly = 259.883;
   neptune.period = 164.79;

   Planet *planets = new Planet[8]{mercury, venus,  earth,  mars,
                                   jupiter, saturn, uranus, neptune};

   return planets;
}


// calcYears() solves for time using the velocity equation
// Pre: distanceAsAU and velocityAsMph are both positive
// Post: a positive double is returned
double calcYears(double distanceAsAU, double velocityAsMph)
{
   const int HOURS_PER_YEAR = 8760;
   const double MILES_PER_AU = 9.296e+7;

   const double distanceAsMiles = distanceAsAU * MILES_PER_AU;

   return distanceAsMiles / velocityAsMph / HOURS_PER_YEAR;
}


// calcTotalTime() loops through the array and calculates each distance
// value, returning the sum
// Pre: planets is a Planet struct pointer
// Post: totalTime is double
double calcTotalTime(Waypoint *&waypoints)
{
   int validIndex = 0;
   double totalTime = 0;

   while (waypoints[validIndex].planetName != "" &&
          waypoints[validIndex].velocityAsMph != 0) {

      totalTime += waypoints[validIndex].timeAsYears;
      validIndex += 1;
   }

   return totalTime;
}


// Numerical approximation of Eccentric Anomaly (E) using the Newton-Raphson
// method
// Pre: will only converge for elliptical orbits (i.e. the eccentricity is NOT
// near 1)
// Post: Solve for E within 4-5 digits of accuracy with a max of 17-18
// iterations - source: https://en.wikipedia.org/wiki/Kepler%27s_equation
// Notes: for this function and calcHeliocentricCord() I used single character
// variable names. I know this is typically not advised. In this case, the
// convention matches the equations in my reference material and made it easier
// to comprehend each step.
double calcEccentricAnomaly(double eccentricity, double normalizedMeanAnomaly)
{
   const double e = eccentricity;
   const double M = normalizedMeanAnomaly;
   auto isConverging = [](int count) { return count < 19; };

   // the inverse of the standard form of Kepler's equation
   double E = M + e * sin(M) * (1 + e * cos(M));

   double delta;
   int iterationCount = 0;

   do {
      const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
      delta = abs(E1 - E);
      E = E1;
      iterationCount++;
   } while (delta >= 0.0001 && isConverging(iterationCount));

   // failsafe, should never happen with current planet selection
   if (!isConverging(iterationCount)) {
      print("calcEccentricAnomaly() failed. unable to converge.");
      print("delta: " + to_string(delta));
      exit(1);
   }

   return E;
}


// calcHeliocentricCord() calculates the heliocentric coordinates of the planet
// at the specified time
// Pre: Planet is a planet struct, daysSinceEpoch is an int
// Post: Cord struct is returned
Cord calcHeliocentricCord(const Planet &planet, int daysSinceEpoch)
{
   const double meanMotion = 360.0 / planet.period;
   const double normalizedMeanAnomaly =
       normalizeDegrees(planet.meanAnomaly + meanMotion * daysSinceEpoch);

   // orbital elements normalized to J2000
   const double a = planet.semiMajorAxis;
   const double e = planet.eccentricity;
   const double o = toRadians(planet.longitudeOfAscendingNode);
   const double p = toRadians(planet.longitudeOfPerihelion);
   const double i = toRadians(planet.orbitalInclination);

   // normalized to specified date
   const double M = toRadians(normalizedMeanAnomaly);
   const double E = calcEccentricAnomaly(e, M);

   // position in 2d orbital plane
   const double xv = a * (cos(E) - e);
   const double yv = a * (sqrt(1.0 - e * e) * sin(E));

   // The True Anomaly (v) is the final angle we need to define the position
   // of a satellite in orbit, the other two being Eccentric Anomaly (E) and
   // Mean Anomaly (M).
   const double v = atan2(yv, xv);

   // The radius vector (r) is the distance of the satellite to the focal point
   // of the ellipse, in this case the sun.
   const double r = sqrt(xv * xv + yv * yv);

   // heliocentric 3d cartesian coordinates
   const double xh =
       r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
   const double yh =
       r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
   const double zh = r * (sin(v + p - o) * sin(i));

   return {xh, yh, zh};
}


// createWaypoint() creates a Waypoint struct that includes inputs and the
// results attributed to one travel entry. Acts as a controller
// Pre: planets is an array of Planet structs
// Post: a Waypoint struct is returned
Waypoint createWaypoint(const Planet *&planets)
{
   const Date date = getDate();
   const int planetIndex = getPlanetIndex();
   const double velocityAsMph = getVelocityAsMph();

   const int days = calcDaysSinceEpoch(date);

   const Planet planet = planets[planetIndex];
   const Planet earth = planets[2];

   const Cord heliocentricCord = calcHeliocentricCord(planet, days);
   const Cord heliocentricCordEarth = calcHeliocentricCord(earth, days);

   // geocentric 3d cartesian coordinates
   const double gX = heliocentricCordEarth.x - heliocentricCord.x;
   const double gY = heliocentricCordEarth.y - heliocentricCord.y;
   const double gZ = heliocentricCordEarth.z - heliocentricCord.z;

   const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));
   const double years = calcYears(distance, velocityAsMph);
   const string totalTimeAsString = formatTimeResult("Travel time", years);

   print(totalTimeAsString);

   return {date, planet.name, velocityAsMph, distance, years};
}
