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

struct Cord {
  double x;
  double y;
  double z;
};

struct Planet {
  double m_longitudeOfAscendingNode;
  double m_orbitalInclination;
  double m_semimajorAxis;
  double m_eccentricity;
  double m_motion;
  double m_longitudeOfPerihelion;
  double m_anomalyAtEpoch;
  double anomalisticYear;
  double m_anomaly;
};

// convert to scalar between 0 and 360 inclusive.
double toAngle(double scalar) {
  float mod = remainder(scalar, 360);
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

Cord getPlanetStats(const Planet& planet) {
  const double a = planet.m_semimajorAxis;
  const double e = planet.m_eccentricity;
  const double M = planet.m_anomaly;
  const double o = toRadians(planet.m_longitudeOfAscendingNode);
  const double p = toRadians(planet.m_longitudeOfPerihelion);
  const double i = toRadians(planet.m_orbitalInclination);

  const double trueAnomaly =
      M - (180.0 / M_PI) * ((2 * e - pow(e, 3) / 4) * sin(M) +
                            (5.0 / 4.0) * pow(e, 2) * sin(2 * M) +
                            (13.0 / 12.0) * pow(e, 3) * sin(3 * M));

  const double v = toRadians(trueAnomaly);
  // cout << toAngle(trueAnomaly) << endl;

  const double r = a * (1 - pow(e, 2)) / (1 + e * cos(v));

  const float x =
      r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
  const float y =
      r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
  const float z = r * (sin(v + p - o) * sin(i));

  display(trueAnomaly, "trueAnomaly");
  display(r, "radiusVector");

  return {x, y, z};
}

int main() {
  const double dayNum = getDayNum();

  Planet mars;
  mars.m_longitudeOfAscendingNode = 49.57854;
  mars.m_orbitalInclination = 1.85061;
  mars.m_semimajorAxis = 1.52366231;
  mars.m_eccentricity = 0.09341233;
  mars.m_anomalyAtEpoch = 19.387;
  mars.anomalisticYear = 686.9957;
  mars.m_longitudeOfPerihelion = 336.04084;
  mars.m_motion = 360 / mars.anomalisticYear;
  mars.m_anomaly = toAngle(mars.m_anomalyAtEpoch + mars.m_motion * dayNum);

  Planet venus;
  venus.m_longitudeOfAscendingNode = 76.68069;
  venus.m_orbitalInclination = 3.39471;
  venus.m_semimajorAxis = 0.72333199;
  venus.m_eccentricity = 0.00677323;
  venus.m_longitudeOfPerihelion = 131.53298;
  venus.m_anomalyAtEpoch = 50.45;
  venus.anomalisticYear = 224.7008;
  venus.m_motion = 360 / venus.anomalisticYear;
  venus.m_anomaly = toAngle(venus.m_anomalyAtEpoch + venus.m_motion * dayNum);

  Planet mercury;
  mercury.m_longitudeOfAscendingNode = 48.33167;
  mercury.m_orbitalInclination = 7.00487;
  mercury.m_semimajorAxis = 0.38709893;
  mercury.m_eccentricity = 0.20563069;
  mercury.m_longitudeOfPerihelion = 77.45645;
  mercury.m_anomalyAtEpoch = 174.796;
  mercury.anomalisticYear = 87.969;
  mercury.m_motion = 360 / mercury.anomalisticYear;
  mercury.m_anomaly =
      toAngle(mercury.m_anomalyAtEpoch + mercury.m_motion * dayNum);

  Planet earth;
  earth.m_longitudeOfAscendingNode = -11.26064;
  earth.m_orbitalInclination = 0.00005;
  earth.m_semimajorAxis = 1.00000011;
  earth.m_eccentricity = 0.01671022;
  earth.m_longitudeOfPerihelion = 102.94719;
  earth.m_anomalyAtEpoch = 357.51716;
  earth.anomalisticYear = 365.259636;
  earth.m_motion = 360 / earth.anomalisticYear;
  earth.m_anomaly = toAngle(earth.m_anomalyAtEpoch + earth.m_motion * dayNum);

  // double eccentricAnomaly =
  //     m_anomaly + m_eccentricity * (180 / M_PI) * sin(m_anomaly) *
  //                       (1.0 + m_eccentricity * cos(m_anomaly));

  // double delta = .5;

  // while (abs(delta) >= .0001) {
  //   delta =
  //       eccentricAnomaly - m_eccentricity * sin(eccentricAnomaly) -
  //       m_anomaly;
  //   eccentricAnomaly =
  //       (eccentricAnomaly - delta) / (1 - m_eccentricity *
  //       cos(eccentricAnomaly));
  // }

  Cord earthCord = getPlanetStats(earth);
  Cord marsCord = getPlanetStats(mars);

  const double gX = marsCord.x - earthCord.x;
  const double gY = marsCord.y - earthCord.y;
  const double gZ = marsCord.z - earthCord.z;

  const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

  // display(gX, "gX");
  // display(gY, "gY");
  // display(gZ, "gZ");

  // const double obliquity = -23.439292;

  // const double qX = gX;
  // const double qY = gY * cos(obliquity) - gZ * sin(obliquity);
  // const double qZ = gY * sin(obliquity) + gZ * cos(obliquity);

  // const double distance = sqrt(pow(qX, 2) + pow(qY, 2) + pow(qZ, 2));

  display(distance, "distance");

  // display(x, "x");
}

// 84.78
