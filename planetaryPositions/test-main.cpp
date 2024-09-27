#include <math.h>

#include <iostream>
#include <string>

using namespace std;

void display(const string& output, string label = "", bool cReturn = 1) {
  if (label != "") {
    label += ": ";
  }
  if (cReturn) {
    cout << label << output << endl;
    return;
  }

  cout << label << output << flush;
}

void display(double output, string label = "", bool cReturn = 1) {
  if (label != "") {
    label += ": ";
  }
  if (cReturn) {
    cout << label << output << endl;
    return;
  }

  cout << label << output << flush;
}

struct Cord {
  double x;
  double y;
  double z;
};

struct OrbitalElements_J2000 {
  double longitudeOfAscendingNode;
  double orbitalInclination;
  double semimajorAxis;
  double eccentricity;
  double longitudeOfPerihelion;
  double meanAnomaly;
  double period;
};

// convert to scalar between 0 and 360 inclusive.
double normalizeAngle(double scalar) {
  double mod = remainder(scalar, 360);
  if (mod < 0) {
    mod += 360;
  }
  return mod;
}

double getDayNum() {
  const int year = 2024;
  const int month = 9;
  const int day = 26;
  const double universalTime = 13.10;

  // intentional integer division
  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;

  totDays -= universalTime / 24.0;

  return totDays;
}

double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

Cord getHeliocentricCords(const OrbitalElements_J2000& body, int dayNum) {
  const double diurnalMotion = 360 / body.period;
  const double meanAnomaly =
      normalizeAngle(body.meanAnomaly + diurnalMotion * dayNum);

  const double a = body.semimajorAxis;
  const double e = body.eccentricity;
  const double M = toRadians(meanAnomaly);
  const double o = toRadians(body.longitudeOfAscendingNode);
  const double p = toRadians(body.longitudeOfPerihelion);
  const double i = toRadians(body.orbitalInclination);

  // Initial approximation of Eccentric MeanAnomaly (E) using Kepler's equation
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

  // Calculate true meanAnomaly (v) and radius vector (r)
  const double v = atan2(yv, xv);
  const double r = sqrt(xv * xv + yv * yv);
  const double trueMeanAnomaly = (180.0 / M_PI) * v;

  // Convert to 3D coordinates in space
  const double x =
      r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
  const double y =
      r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
  const double z = r * (sin(v + p - o) * sin(i));

  // Displaying true meanAnomaly and radius vector for debugging
  display(trueMeanAnomaly, "trueMeanAnomaly");
  display(r, "radiusVector");

  // Return the position coordinates in space
  return {x, y, z};
}

int main() {
  const double dayNum = getDayNum();

  OrbitalElements_J2000 mercury;
  mercury.semimajorAxis = 0.38709893;
  mercury.eccentricity = 0.20563069;
  mercury.orbitalInclination = 7.00487;
  mercury.longitudeOfAscendingNode = 48.33167;
  mercury.longitudeOfPerihelion = 77.45645;
  mercury.meanAnomaly = 174.796;
  mercury.period = 87.969;

  OrbitalElements_J2000 venus;
  venus.semimajorAxis = 0.72333199;
  venus.eccentricity = 0.00677323;
  venus.orbitalInclination = 3.39471;
  venus.longitudeOfAscendingNode = 76.68069;
  venus.longitudeOfPerihelion = 131.53298;
  venus.meanAnomaly = 50.45;
  venus.period = 224.7008;

  OrbitalElements_J2000 mars;
  mars.semimajorAxis = 1.52366231;
  mars.eccentricity = 0.09341233;
  mars.orbitalInclination = 1.85061;
  mars.longitudeOfAscendingNode = 49.57854;
  mars.longitudeOfPerihelion = 336.04084;
  mars.meanAnomaly = 19.387;
  mars.period = 686.9957;

  OrbitalElements_J2000 earth;
  earth.semimajorAxis = 1.00000011;
  earth.eccentricity = 0.01671022;
  earth.orbitalInclination = 0.00005;
  earth.longitudeOfAscendingNode = -11.26064;
  earth.longitudeOfPerihelion = 102.94719;
  earth.meanAnomaly = 357.51716;
  earth.period = 365.259636;

  OrbitalElements_J2000 jupiter;
  jupiter.semimajorAxis = 5.20336301;
  jupiter.eccentricity = 0.04839266;
  jupiter.orbitalInclination = 1.30530;
  jupiter.longitudeOfAscendingNode = 100.55615;
  jupiter.longitudeOfPerihelion = 14.75385;
  jupiter.meanAnomaly = 20.020;
  jupiter.period = 11.862;

  OrbitalElements_J2000 saturn;
  saturn.semimajorAxis = 9.53707032;
  saturn.eccentricity = 0.05415060;
  saturn.orbitalInclination = 2.48446;
  saturn.longitudeOfAscendingNode = 113.71504;
  saturn.longitudeOfPerihelion = 92.43194;
  saturn.meanAnomaly = 317.020;
  saturn.period = 29.4475;

  OrbitalElements_J2000 uranus;
  uranus.semimajorAxis = 19.19126393;
  uranus.eccentricity = 0.04716771;
  uranus.orbitalInclination = 0.76986;
  uranus.longitudeOfAscendingNode = 74.22988;
  uranus.longitudeOfPerihelion = 170.96424;
  uranus.meanAnomaly = 142.238600;
  uranus.period = 84.011;

  OrbitalElements_J2000 neptune;
  neptune.semimajorAxis = 30.06896348;
  neptune.eccentricity = 0.00858587;
  neptune.orbitalInclination = 1.76917;
  neptune.longitudeOfAscendingNode = 131.72169;
  neptune.longitudeOfPerihelion = 44.97135;
  neptune.meanAnomaly = 259.883;
  neptune.period = 164.79;

  Cord earthCord = getHeliocentricCords(earth, dayNum);
  Cord marsCord = getHeliocentricCords(neptune, dayNum);

  const double gX = marsCord.x - earthCord.x;
  const double gY = marsCord.y - earthCord.y;
  const double gZ = marsCord.z - earthCord.z;

  const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

  display(distance, "distance");
}
