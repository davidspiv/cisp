// https://stjarnhimlen.se/comp/ppcomp.html#5a

#include <cmath>
#include <iostream>
#include <string>

struct CelestialBody {
  double longitudeOfAscendingNode;
  double planeOfEarthsOrbit;
  double argumentOfPerihelion;
  double meanDistanceFromSun;
  double eccentricity;
  double meanAnomaly;
};

const int year = 2024;
const int month = 9;
const int date = 21;

const double dayNumber = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                         3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                         275 * month / 9 + date - 730515;

const double milePerAstronomicalUnit = 9.296e-7;

double toRadians(double degrees) { return degrees * (M_PI / 180); }

double getDistanceAsMilesFromSun() {
  CelestialBody sun;

  sun.longitudeOfAscendingNode = 0.0;
  sun.planeOfEarthsOrbit = 0.0;
  sun.argumentOfPerihelion = 282.9404 + 4.70935E-5 * dayNumber;
  sun.meanDistanceFromSun = 1.000000;
  sun.eccentricity = 0.016709 - 1.151E-9 * dayNumber;
  sun.meanAnomaly = 356.0470 + 0.9856002585 * dayNumber;

  while (sun.meanAnomaly > 360) {
    sun.meanAnomaly -= 360;
  }

  while (sun.meanAnomaly < 0) {
    sun.meanAnomaly += 360;
  };

  const double eccentricAnomaly =
      sun.meanAnomaly + sun.eccentricity * (180 / M_PI) * sin(sun.meanAnomaly) *
                            (1.0 + sun.eccentricity * cos(sun.meanAnomaly));

  const double xAnomaly = cos(eccentricAnomaly) - sun.eccentricity;
  const double yAnomaly =
      sqrt(1.0 - sun.eccentricity * sun.eccentricity) * sin(eccentricAnomaly);

  const double distanceAsAU = sqrt(xAnomaly * xAnomaly + yAnomaly * yAnomaly);
  const double distanceAsMiles = distanceAsAU * milePerAstronomicalUnit;

  return distanceAsMiles;
}

int main() {
  CelestialBody mercury;
  CelestialBody venus;
  CelestialBody mars;
  CelestialBody jupiter;
  CelestialBody saturn;
  CelestialBody uranus;
  CelestialBody neptune;

  const float distanceSun = getDistanceAsMilesFromSun();
  std::cout << "getDistanceAsMilesFromSun: " << distanceSun << std::endl;

  mercury.longitudeOfAscendingNode = 48.3313 + 3.24587E-5 * dayNumber;
  mercury.planeOfEarthsOrbit = 7.0047 + 5.00E-8 * dayNumber;
  mercury.argumentOfPerihelion = 29.1241 + 1.01444E-5 * dayNumber;
  mercury.meanDistanceFromSun = 0.387098;
  mercury.eccentricity = 0.205635 + 5.59E-10 * dayNumber;
  mercury.meanAnomaly = 168.6562 + 4.0923344368 * dayNumber;

  venus.longitudeOfAscendingNode = 76.6799 + 2.46590E-5 * dayNumber;
  venus.planeOfEarthsOrbit = 3.3946 + 2.75E-8 * dayNumber;
  venus.argumentOfPerihelion = 54.8910 + 1.38374E-5 * dayNumber;
  venus.meanDistanceFromSun = 0.723330;
  venus.eccentricity = 0.006773 - 1.302E-9 * dayNumber;
  venus.meanAnomaly = 48.0052 + 1.6021302244 * dayNumber;

  mars.longitudeOfAscendingNode = 49.5574 + 2.11081E-5 * dayNumber;
  mars.planeOfEarthsOrbit = 1.8497 - 1.78E-8 * dayNumber;
  mars.argumentOfPerihelion = 286.5016 + 2.92961E-5 * dayNumber;
  mars.meanDistanceFromSun = 1.523688;
  mars.eccentricity = 0.093405 + 2.516E-9 * dayNumber;
  mars.meanAnomaly = 18.6021 + 0.5240207766 * dayNumber;

  jupiter.longitudeOfAscendingNode = 100.4542 + 2.76854E-5 * dayNumber;
  jupiter.planeOfEarthsOrbit = 1.3030 - 1.557E-7 * dayNumber;
  jupiter.argumentOfPerihelion = 273.8777 + 1.64505E-5 * dayNumber;
  jupiter.meanDistanceFromSun = 5.20256;
  jupiter.eccentricity = 0.048498 + 4.469E-9 * dayNumber;
  jupiter.meanAnomaly = 19.8950 + 0.0830853001 * dayNumber;

  saturn.longitudeOfAscendingNode = 113.6634 + 2.38980E-5 * dayNumber;
  saturn.planeOfEarthsOrbit = 2.4886 - 1.081E-7 * dayNumber;
  saturn.argumentOfPerihelion = 339.3939 + 2.97661E-5 * dayNumber;
  saturn.meanDistanceFromSun = 9.55475;
  saturn.eccentricity = 0.055546 - 9.499E-9 * dayNumber;
  saturn.meanAnomaly = 316.9670 + 0.0334442282 * dayNumber;

  uranus.longitudeOfAscendingNode = 74.0005 + 1.3978E-5 * dayNumber;
  uranus.planeOfEarthsOrbit = 0.7733 + 1.9E-8 * dayNumber;
  uranus.argumentOfPerihelion = 96.6612 + 3.0565E-5 * dayNumber;
  uranus.meanDistanceFromSun = 19.18171 - 1.55E-8 * dayNumber;
  uranus.eccentricity = 0.047318 + 7.45E-9 * dayNumber;
  uranus.meanAnomaly = 142.5905 + 0.011725806 * dayNumber;

  neptune.longitudeOfAscendingNode = 131.7806 + 3.0173E-5 * dayNumber;
  neptune.planeOfEarthsOrbit = 1.7700 - 2.55E-7 * dayNumber;
  neptune.argumentOfPerihelion = 272.8461 - 6.027E-6 * dayNumber;
  neptune.meanDistanceFromSun = 30.05826 + 3.313E-8 * dayNumber;
  neptune.eccentricity = 0.008606 + 2.15E-9 * dayNumber;
  neptune.meanAnomaly = 260.2471 + 0.005995147 * dayNumber;

  CelestialBody celestialBody[] = {mercury, venus,  mars,   jupiter,
                                   saturn,  uranus, neptune};

  //    std::cout << dayNumber << std::endl;
}
