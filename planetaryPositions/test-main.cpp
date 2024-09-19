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

void display(float output, string label = "", bool cReturn = 1) {
  if (label != "") {
    label += ": ";
  }
  if (cReturn) {
    cout << label << output << endl;
    return;
  }

  cout << label << output << flush;
}

double getDayNum() {
  const int year = 2024;
  const int month = 9;
  const int day = 13;
  const double universalTime = 13.10;

  // intentional integer division
  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;

  totDays -= universalTime / 24.0;

  return totDays;
}

// convert to scalar between 0 and 360 inclusive.
double toAngle(double scalar) {
  float mod = remainder(scalar, 360);
  if (mod < 0) {
    mod += 360;
  }
  return mod;
}

int main() {
  const double dayNum = getDayNum();

  // const double longitudeOfAscendingNode = 49.57854 + 2.11081E-5 * dayNum;
  // const double orbitalInclination = 1.8497 - 1.78E-8 * dayNum;
  // const double argumentOfPerihelion = 206.650 + 2.92961E-5 * dayNum;
  const double semimajorAxis = 1.52366231;
  const double eccentricity = 0.09341233 + 4.469E-9 * dayNum;
  const double meanAnomalyAsData = 19.387 + 0.5240207766 * dayNum;

  // const double actualDistanceToSun = 1.48;
  // const double actualDistanceToEarth = 1.37;

  const double meanAnomaly = toAngle(meanAnomalyAsData);

  double eccentricAnomaly =
      meanAnomaly + eccentricity * (180 / M_PI) * sin(meanAnomaly) *
                        (1.0 + eccentricity * cos(meanAnomaly));

  double delta = .5;

  while (abs(delta) >= .0001) {
    delta =
        eccentricAnomaly - eccentricity * sin(eccentricAnomaly) - meanAnomaly;
    eccentricAnomaly =
        (eccentricAnomaly - delta) / (1 - eccentricity * cos(eccentricAnomaly));
  }

  const double trueAnomaly =
      meanAnomaly -
      (180 / M_PI) *
          ((2 * eccentricity - pow(eccentricity, 3) / 4) * sin(meanAnomaly) +
           5 / 4 * pow(eccentricity, 2) * sin(2 * meanAnomaly) +
           13 / 12 * pow(eccentricity, 3) * sin(3 * meanAnomaly));

  const double radiusVector = semimajorAxis * (1 - pow(eccentricity, 2)) /
                              (1 + eccentricity * cos(trueAnomaly));

  // const float x = radiusVector *
  //                 (cos(longitudeOfAscendingNode) *
  //                      cos(trueAnomaly + 336.04084 -
  //                      longitudeOfAscendingNode) -
  //                  sin(longitudeOfAscendingNode) *
  //                      sin(trueAnomaly + 336.04084 -
  //                      longitudeOfAscendingNode) * cos(orbitalInclination));
  // const float y = radiusVector *
  //                 (sin(longitudeOfAscendingNode) *
  //                      cos(trueAnomaly + 336.04084 -
  //                      longitudeOfAscendingNode) +
  //                  cos(longitudeOfAscendingNode) *
  //                      sin(trueAnomaly + 336.04084 -
  //                      longitudeOfAscendingNode) * cos(orbitalInclination));
  // const float z =
  //     radiusVector * (sin(trueAnomaly + 336.04084 - longitudeOfAscendingNode)
  //     *
  //                     sin(orbitalInclination));

  display(trueAnomaly, "trueAnomaly");
  display(radiusVector, "radiusVector");
  // display(x, "x");
}

// 76.58
