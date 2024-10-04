
#include <math.h>

#include "./helpers/io.h"

using namespace std;

double normalizeDegrees(double degrees) {
  double normalizedDegrees = remainder(degrees, 360);

  if (normalizedDegrees < 0) {
    normalizedDegrees += 360;
  }

  return normalizedDegrees;
}

struct Planet {
  string name;
  double semiMajorAxis;
  double eccentricity;
  double orbitalInclination;
  double longitudeOfAscendingNode;
  double longitudeOfPerihelion;
  double meanAnomaly;
  double period;
  double mass;
};

int main() {
  Planet mercury;
  mercury.name = "mercury";
  mercury.semiMajorAxis = 0.38709893;
  mercury.eccentricity = 0.20563069;
  mercury.orbitalInclination = 7.00487;
  mercury.longitudeOfAscendingNode = 48.33167;
  mercury.longitudeOfPerihelion = 77.45645;
  mercury.meanAnomaly = 174.796;
  mercury.period = 87.969;
  mercury.mass = 3.285e+23;

  const double areaElipse =
      pow(mercury.semiMajorAxis, 2) * sqrt(1 - pow(mercury.eccentricity, 2));

  // Kepler's Third Law: The square of the period of an object's orbit is
  // proportional to the cube of its semi-major axis

  const double auToM = 1.496e+11;
  const double secondsToYears = 1.15741e-5;
  const double sunMass = 1.9891e+30;
  const double G = 6.673e-11;

  const double proportionalityConstant =
      (4 * pow(M_PI, 2)) / (G * (mercury.mass + sunMass));

  const double period =
      sqrt(proportionalityConstant * pow(mercury.semiMajorAxis * auToM, 3));

  // print(period * secondsToYears);

  const double velocity = sqrt(G * sunMass / 4.6e+10);
  // const double velocity = 2 * M_PI * 47e+7 *  period;

  print(velocity / 1000);

  return 0;
}
