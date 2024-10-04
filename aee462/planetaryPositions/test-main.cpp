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

struct OrbitalElements {
  string name;
  double semiMajorAxis_AU;
  double eccentricity;
  double orbitalInclination_Deg;
  double longitudeOfAscendingNode_Deg;
  double longitudeOfPerihelion_Deg;
  double meanAnomaly_Deg;
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
  const int day = 9;
  const double universalTime = 13.10;

  // intentional integer division
  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;

  totDays -= universalTime / 24.0;

  return totDays;
}

double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

// double radiusVectors[] = {0.0, 0.0, 0.0};

Cord getHeliocentricCords(const OrbitalElements& body, int dayNum) {
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

int main() {
  const double dayNum = getDayNum();

  OrbitalElements mercury;
  mercury.name = "mercury";
  mercury.semiMajorAxis_AU = 0.38709893;
  mercury.eccentricity = 0.20563069;
  mercury.orbitalInclination_Deg = 7.00487;
  mercury.longitudeOfAscendingNode_Deg = 48.33167;
  mercury.longitudeOfPerihelion_Deg = 77.45645;
  mercury.meanAnomaly_Deg = 174.796;
  mercury.period = 87.969;

  OrbitalElements venus;
  venus.name = "venus";
  venus.semiMajorAxis_AU = 0.72333199;
  venus.eccentricity = 0.00677323;
  venus.orbitalInclination_Deg = 3.39471;
  venus.longitudeOfAscendingNode_Deg = 76.68069;
  venus.longitudeOfPerihelion_Deg = 131.53298;
  venus.meanAnomaly_Deg = 50.45;
  venus.period = 224.7008;

  OrbitalElements earth;
  earth.name = "earth";
  earth.semiMajorAxis_AU = 1.00000011;
  earth.eccentricity = 0.01671022;
  earth.orbitalInclination_Deg = 0.00005;
  earth.longitudeOfAscendingNode_Deg = -11.26064;
  earth.longitudeOfPerihelion_Deg = 102.94719;
  earth.meanAnomaly_Deg = 357.51716;
  earth.period = 365.259636;

  OrbitalElements mars;
  mars.name = "mars";
  mars.semiMajorAxis_AU = 1.52366231;
  mars.eccentricity = 0.09341233;
  mars.orbitalInclination_Deg = 1.85061;
  mars.longitudeOfAscendingNode_Deg = 49.57854;
  mars.longitudeOfPerihelion_Deg = 336.04084;
  mars.meanAnomaly_Deg = 19.387;
  mars.period = 686.9957;

  OrbitalElements jupiter;
  jupiter.name = "jupiter";
  jupiter.semiMajorAxis_AU = 5.20336301;
  jupiter.eccentricity = 0.04839266;
  jupiter.orbitalInclination_Deg = 1.30530;
  jupiter.longitudeOfAscendingNode_Deg = 100.55615;
  jupiter.longitudeOfPerihelion_Deg = 14.75385;
  jupiter.meanAnomaly_Deg = 20.020;
  jupiter.period = 11.862;

  OrbitalElements saturn;
  saturn.name = "saturn";
  saturn.semiMajorAxis_AU = 9.53707032;
  saturn.eccentricity = 0.05415060;
  saturn.orbitalInclination_Deg = 2.48446;
  saturn.longitudeOfAscendingNode_Deg = 113.71504;
  saturn.longitudeOfPerihelion_Deg = 92.43194;
  saturn.meanAnomaly_Deg = 317.020;
  saturn.period = 29.4475;

  OrbitalElements uranus;
  uranus.name = "uranus";
  uranus.semiMajorAxis_AU = 19.19126393;
  uranus.eccentricity = 0.04716771;
  uranus.orbitalInclination_Deg = 0.76986;
  uranus.longitudeOfAscendingNode_Deg = 74.22988;
  uranus.longitudeOfPerihelion_Deg = 170.96424;
  uranus.meanAnomaly_Deg = 142.238600;
  uranus.period = 84.011;

  OrbitalElements neptune;
  neptune.name = "neptune";
  neptune.semiMajorAxis_AU = 30.06896348;
  neptune.eccentricity = 0.00858587;
  neptune.orbitalInclination_Deg = 1.76917;
  neptune.longitudeOfAscendingNode_Deg = 131.72169;
  neptune.longitudeOfPerihelion_Deg = 44.97135;
  neptune.meanAnomaly_Deg = 259.883;
  neptune.period = 164.79;

  OrbitalElements planets[] = {mercury, venus,  earth,  mars,
                               jupiter, saturn, uranus, neptune};

  const size_t size = sizeof(planets) / sizeof(*planets);

  Cord heliocentricCords[size];

  for (size_t i = 0; i < size; i++) {
    heliocentricCords[i] = getHeliocentricCords(planets[i], dayNum);
  }

  cout << endl;

  for (size_t i = 0; i < size; i++) {
    const double gX = heliocentricCords[2].x - heliocentricCords[i].x;
    const double gY = heliocentricCords[2].y - heliocentricCords[i].y;
    const double gZ = heliocentricCords[2].z - heliocentricCords[i].z;

    const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

    cout << "From earth (AU) " << planets[i].name << ": " << distance << endl;
  }
}
