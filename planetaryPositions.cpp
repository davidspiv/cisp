// https://stjarnhimlen.se/comp/ppcomp.html#5a
// http://www.stargazing.net/kepler/ellipse.html

#include <cmath>
#include <iostream>
#include <string>

struct CelestialBody {
  std::string name;
  double longitudeOfAscendingNode;
  double orbitalInclination;
  double argumentOfPerihelion;
  double semimajorAxis;
  double eccentricity;
  double meanAnomaly;
};

struct SunVector {
  double xs;
  double ys;
};

struct EclipticComponents {
  std::string name;
  double longitudeEclipse;
  double latitudeEclipse;
  double radiusVector;
};

const double milePerAstronomicalUnit = 9.296e-7;

// Days from 1/1/2000
double getDayNum() {
  const int year = 2024;
  const int month = 9;
  const int day = 2;
  const double universalTime = 0.0;

  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;

  totDays += universalTime / 24.0;

  return totDays;
}

const double dayNum = getDayNum();

// the "tilt" of the Earth's axis of rotation
const double eclipticObliquity = 23.4393 - 3.563E-7 * dayNum;

// convert to scalar between 0 and 360 inclusive.
double toAngle(double scalar) {
  double degree = fmod(scalar, 360);
  if (degree < 0) {
    degree += 360;
  }
  return degree;
}

// Geocentric coordinates of the sun
SunVector getSunVector() {
  SunVector sunVector;

  const double argumentOfPerihelion = 282.9404 + 4.70935E-5 * dayNum;
  const double eccentricity = 0.016709 - 1.151E-9 * dayNum;

  const double meanAnomaly = toAngle(356.0470 + 0.9856002585 * dayNum);

  const double eccentricAnomaly =
      meanAnomaly + eccentricity * (180 / M_PI) * sin(meanAnomaly) *
                        (1.0 + eccentricity * cos(meanAnomaly));

  const double xAnomaly = cos(eccentricAnomaly) - eccentricity;
  const double yAnomaly =
      sqrt(1.0 - eccentricity * eccentricity) * sin(eccentricAnomaly);

  const double trueAnomaly = atan2(yAnomaly, xAnomaly);
  const double distanceAsAU = sqrt(xAnomaly * xAnomaly + yAnomaly * yAnomaly);

  const double lonsun = trueAnomaly + argumentOfPerihelion;

  sunVector.xs = distanceAsAU * cos(lonsun);
  sunVector.ys = distanceAsAU * sin(lonsun);

  return sunVector;
}

EclipticComponents getEclipticComponents(CelestialBody &body) {
  body.meanAnomaly = toAngle(body.meanAnomaly);

  double eccentricAnomaly =
      body.meanAnomaly + body.eccentricity * (180 / M_PI) *
                             sin(body.meanAnomaly) *
                             (1.0 + body.eccentricity * cos(body.meanAnomaly));

  if (body.eccentricity > 0.05) {
    int count = 0;
    double difference;
    double E0 = eccentricAnomaly;
    double E1;
    do {
      if (count > 10) {
        std::cout << body.name << " not converging" << std::endl;
        break;
      };
      E1 = E0 - (E0 - body.eccentricity * (180 / M_PI) * sin(E0) -
                 body.meanAnomaly) /
                    (1 - body.eccentricity * cos(E0));

      difference = std::abs(E1 - eccentricAnomaly);
      count++;
      E0 = E1;

    } while (difference > 0.001);

    if (difference <= 0.001) {
      eccentricAnomaly = E1;
    }
  }

  const double xv =
      body.semimajorAxis * (cos(eccentricAnomaly) - body.eccentricity);

  const double yv = body.semimajorAxis *
                    sqrt(1.0 - body.eccentricity * body.eccentricity) *
                    sin(eccentricAnomaly);

  const double trueAnomaly = atan2(yv, xv);

  const double radiusVector = sqrt(xv * xv + yv * yv);

  const double xh =
      radiusVector * (cos(body.longitudeOfAscendingNode) *
                          cos(trueAnomaly + body.argumentOfPerihelion) -
                      sin(body.longitudeOfAscendingNode) *
                          sin(trueAnomaly + body.argumentOfPerihelion) *
                          cos(body.orbitalInclination));
  const double yh =
      radiusVector * (sin(body.longitudeOfAscendingNode) *
                          cos(trueAnomaly + body.argumentOfPerihelion) +
                      cos(body.longitudeOfAscendingNode) *
                          sin(trueAnomaly + body.argumentOfPerihelion) *
                          cos(body.orbitalInclination));
  const double zh =
      radiusVector * (sin(trueAnomaly + body.argumentOfPerihelion) *
                      sin(body.orbitalInclination));

  EclipticComponents eclipticComponents;
  eclipticComponents.name = body.name;
  eclipticComponents.longitudeEclipse = atan2(yh, xh);
  eclipticComponents.latitudeEclipse = atan2(zh, sqrt(xh * xh + yh * yh));
  eclipticComponents.radiusVector = radiusVector;

  return eclipticComponents;
}

int main() {
  CelestialBody mercury;
  CelestialBody venus;
  CelestialBody mars;
  CelestialBody jupiter;
  CelestialBody saturn;
  CelestialBody uranus;
  CelestialBody neptune;

  mercury.name = "mercury";
  mercury.longitudeOfAscendingNode = 48.3313 + 3.24587E-5 * dayNum;
  mercury.orbitalInclination = 7.0047 + 5.00E-8 * dayNum;
  mercury.argumentOfPerihelion = 29.1241 + 1.01444E-5 * dayNum;
  mercury.semimajorAxis = 0.387098;
  mercury.eccentricity = 0.205635 + 5.59E-10 * dayNum;
  mercury.meanAnomaly = 168.6562 + 4.0923344368 * dayNum;

  venus.name = "venus";
  venus.longitudeOfAscendingNode = 76.6799 + 2.46590E-5 * dayNum;
  venus.orbitalInclination = 3.3946 + 2.75E-8 * dayNum;
  venus.argumentOfPerihelion = 54.8910 + 1.38374E-5 * dayNum;
  venus.semimajorAxis = 0.723330;
  venus.eccentricity = 0.006773 - 1.302E-9 * dayNum;
  venus.meanAnomaly = 48.0052 + 1.6021302244 * dayNum;

  mars.name = "mars";
  mars.longitudeOfAscendingNode = 49.5574 + 2.11081E-5 * dayNum;
  mars.orbitalInclination = 1.8497 - 1.78E-8 * dayNum;
  mars.argumentOfPerihelion = 286.5016 + 2.92961E-5 * dayNum;
  mars.semimajorAxis = 1.523688;
  mars.eccentricity = 0.093405 + 2.516E-9 * dayNum;
  mars.meanAnomaly = 18.6021 + 0.5240207766 * dayNum;

  jupiter.name = "jupiter";
  jupiter.longitudeOfAscendingNode = 100.4542 + 2.76854E-5 * dayNum;
  jupiter.orbitalInclination = 1.3030 - 1.557E-7 * dayNum;
  jupiter.argumentOfPerihelion = 273.8777 + 1.64505E-5 * dayNum;
  jupiter.semimajorAxis = 5.20256;
  jupiter.eccentricity = 0.048498 + 4.469E-9 * dayNum;
  jupiter.meanAnomaly = 19.8950 + 0.0830853001 * dayNum;

  saturn.name = "saturn";
  saturn.longitudeOfAscendingNode = 113.6634 + 2.38980E-5 * dayNum;
  saturn.orbitalInclination = 2.4886 - 1.081E-7 * dayNum;
  saturn.argumentOfPerihelion = 339.3939 + 2.97661E-5 * dayNum;
  saturn.semimajorAxis = 9.55475;
  saturn.eccentricity = 0.055546 - 9.499E-9 * dayNum;
  saturn.meanAnomaly = 316.9670 + 0.0334442282 * dayNum;

  uranus.name = "uranus";
  uranus.longitudeOfAscendingNode = 74.0005 + 1.3978E-5 * dayNum;
  uranus.orbitalInclination = 0.7733 + 1.9E-8 * dayNum;
  uranus.argumentOfPerihelion = 96.6612 + 3.0565E-5 * dayNum;
  uranus.semimajorAxis = 19.18171 - 1.55E-8 * dayNum;
  uranus.eccentricity = 0.047318 + 7.45E-9 * dayNum;
  uranus.meanAnomaly = 142.5905 + 0.011725806 * dayNum;

  neptune.name = "neptune";
  neptune.longitudeOfAscendingNode = 131.7806 + 3.0173E-5 * dayNum;
  neptune.orbitalInclination = 1.7700 - 2.55E-7 * dayNum;
  neptune.argumentOfPerihelion = 272.8461 - 6.027E-6 * dayNum;
  neptune.semimajorAxis = 30.05826 + 3.313E-8 * dayNum;
  neptune.eccentricity = 0.008606 + 2.15E-9 * dayNum;
  neptune.meanAnomaly = 260.2471 + 0.005995147 * dayNum;

  CelestialBody celestialBody[7] = {mercury, venus,  mars,   jupiter,
                                    saturn,  uranus, neptune};

  SunVector sunVector = getSunVector();
  EclipticComponents eclipticComponents[7];

  double Mj;
  double Ms;
  double Mu;

  // The position in space
  for (size_t i = 1; i < 7; i++) {
    eclipticComponents[i] = getEclipticComponents(celestialBody[i]);

    if (celestialBody[i].name == "jupiter") {
      Mj = celestialBody[i].meanAnomaly;
    }
    if (celestialBody[i].name == "saturn") {
      Ms = celestialBody[i].meanAnomaly;
    }
    if (celestialBody[i].name == "uranus") {
      Mu = celestialBody[i].meanAnomaly;
    }
  }

  // Perturbations of Jupiter, Saturn and Uranus
  for (size_t i = 1; i < 7; i++) {
    if (eclipticComponents[i].name == "jupiter") {
      eclipticComponents[i].longitudeEclipse -=
          0.332 * sin(2 * Mj - 5 * Ms - 67.6);
      eclipticComponents[i].longitudeEclipse -=
          0.056 * sin(2 * Mj - 2 * Ms + 21);
      eclipticComponents[i].longitudeEclipse +=
          0.042 * sin(3 * Mj - 5 * Ms + 21);
      eclipticComponents[i].longitudeEclipse -= 0.036 * sin(Mj - 2 * Ms);
      eclipticComponents[i].longitudeEclipse += 0.022 * cos(Mj - Ms);
      eclipticComponents[i].longitudeEclipse +=
          0.023 * sin(2 * Mj - 3 * Ms + 52);
      eclipticComponents[i].longitudeEclipse -= 0.016 * sin(Mj - 5 * Ms - 69);
    }

    // makes saturn result worse?
    //  if (eclipticComponents[i].name == "saturn") {
    //    eclipticComponents[i].longitudeEclipse +=
    //        0.812 * sin(2 * Mj - 5 * Ms - 67.6);
    //    eclipticComponents[i].longitudeEclipse -=
    //        0.229 * cos(2 * Mj - 4 * Ms - 2);
    //    eclipticComponents[i].longitudeEclipse += 0.119 * sin(Mj - 2 * Ms -
    //    3); eclipticComponents[i].longitudeEclipse +=
    //        0.046 * sin(2 * Mj - 6 * Ms - 69);
    //    eclipticComponents[i].longitudeEclipse += 0.014 * sin(Mj - 3 * Ms +
    //    32);

    //   eclipticComponents[i].latitudeEclipse -= 0.020 * cos(2 * Mj - 4 * Ms -
    //   2); eclipticComponents[i].latitudeEclipse +=
    //       0.018 * sin(2 * Mj - 6 * Ms - 49);
    // }

    if (eclipticComponents[i].name == "uranus") {
      eclipticComponents[i].longitudeEclipse += 0.040 * sin(Ms - 2 * Mu + 6);
      eclipticComponents[i].longitudeEclipse += 0.035 * sin(Ms - 3 * Mu + 33);
      eclipticComponents[i].longitudeEclipse -= 0.015 * sin(Mj - Mu + 20);
    }
  }

  // Geocentric (Earth-centered) coordinates
  for (size_t i = 1; i < 7; i++) {
    const double xh = eclipticComponents[i].radiusVector *
                      cos(eclipticComponents[i].longitudeEclipse) *
                      cos(eclipticComponents[i].latitudeEclipse);
    const double yh = eclipticComponents[i].radiusVector *
                      sin(eclipticComponents[i].longitudeEclipse) *
                      cos(eclipticComponents[i].latitudeEclipse);
    const double zh = eclipticComponents[i].radiusVector *
                      sin(eclipticComponents[i].latitudeEclipse);

    const double xg = xh + sunVector.xs;
    const double yg = yh + sunVector.ys;
    const double zg = zh;

    // Geocentric distance
    const double xe = xg;
    const double ye = yg * cos(eclipticObliquity) - zg * sin(eclipticObliquity);
    const double ze = yg * sin(eclipticObliquity) + zg * cos(eclipticObliquity);

    const double rg = sqrt(xe * xe + ye * ye + ze * ze);

    std::cout << eclipticComponents[i].name << ": " << rg << std::endl;
  }
}
