  // const double mJ = toRadians(planets[4].meanAnomaly_Deg);
  // const double mS = toRadians(planets[5].meanAnomaly_Deg);
  // const double mU = toRadians(planets[6].meanAnomaly_Deg);
  // for (size_t i = 0; i < size; i++) {
  //   double radiusVector;

  //   if (planets[i].name != "jupiter" && planets[i].name != "saturn" &&
  //       planets[i].name != "uranus") {
  //     continue;
  //   }

  //   if (planets[i].name == "jupiter") {
  //     radiusVector = radiusVectors[0];
  //   } else if (planets[i].name == "saturn") {
  //     radiusVector = radiusVectors[1];
  //   } else if (planets[i].name == "uranus") {
  //     radiusVector = radiusVectors[2];
  //   } else {
  //     continue;
  //   }

  //   double x = heliocentricCords[i].x;
  //   double y = heliocentricCords[i].y;
  //   double z = heliocentricCords[i].z;
  //   double lonecl = atan2(y, x);
  //   double latecl = atan2(z, sqrt(x * x + y * y));

  //   if (planets[i].name == "jupiter") {
  //     lonecl -= 0.332 * sin(2 * mJ - 5 * mS - 67.6);
  //     lonecl -= 0.056 * sin(2 * mJ - 2 * mS + 21);
  //     lonecl += 0.042 * sin(3 * mJ - 5 * mS + 21);
  //     lonecl -= 0.036 * sin(mJ - 2 * mS);
  //     lonecl += 0.022 * cos(mJ - mS);
  //     lonecl += 0.023 * sin(2 * mJ - 3 * mS + 52);
  //     lonecl -= 0.016 * sin(mJ - 5 * mS - 69);

  //   } else if (planets[i].name == "saturn") {
  //     lonecl += 0.812 * sin(2 * mJ - 5 * mS - 67.6);
  //     lonecl -= 0.229 * cos(2 * mJ - 4 * mS - 2);
  //     lonecl += 0.119 * sin(mJ - 2 * mS - 3);
  //     lonecl += 0.046 * sin(2 * mJ - 6 * mS - 69);
  //     lonecl += 0.014 * sin(mJ - 3 * mS + 32);

  //     latecl -= 0.020 * cos(2 * mJ - 4 * mS - 2);
  //     latecl += 0.018 * sin(2 * mJ - 6 * mS - 49);

  //   } else if (planets[i].name == "uranus") {
  //     lonecl += 0.040 * sin(mS - 2 * mU + 6);
  //     lonecl += 0.035 * sin(mS - 3 * mU + 33);
  //     lonecl -= 0.015 * sin(mJ - mU + 20);
  //   }

  //   x = radiusVector * cos(lonecl) * cos(latecl);
  //   y = radiusVector * sin(lonecl) * cos(latecl);
  //   z = radiusVector * sin(latecl);

  //   heliocentricCords[i] = {x, y, z};
  // }
