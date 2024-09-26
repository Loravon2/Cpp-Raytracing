#pragma once

#include <iostream>
#include <array>
#include <Dense>

#include "defines.h"

class LightIntensity {
private:
  std::array<float, NUM_COL> rgb;

public:
  LightIntensity(std::array<float, NUM_COL> rgb);
  LightIntensity(float red, float green, float blue);
  LightIntensity();

  static LightIntensity white();
  static LightIntensity red();
  static LightIntensity blue();
  static LightIntensity green();
  static LightIntensity black();
  static LightIntensity purple();
  static LightIntensity gold();
  static LightIntensity dark_purple();

  float at(unsigned k);

  void operator+=(const LightIntensity& other);
  friend const LightIntensity operator+(LightIntensity l1, const LightIntensity& l2);
  
  void operator*=(const LightIntensity& other);
  friend const LightIntensity operator*(LightIntensity l1, const LightIntensity& l2);

  void operator*=(float scalar);
  friend const LightIntensity operator*(LightIntensity li, float scalar);
  friend const LightIntensity operator*(float scalar, LightIntensity li);

  friend std::ostream& operator<<(std::ostream& out, const LightIntensity& li);
};

struct ColData {
  LightIntensity ambient;
  LightIntensity diffuse;
  LightIntensity specular;
  LightIntensity reflected;
  LightIntensity refracted;
  float shininess;

  ColData(LightIntensity ambient, LightIntensity diffuse, LightIntensity specular, LightIntensity reflected, LightIntensity refracted, float shininess);
  ColData();

  static ColData shiny();
  static ColData red_flat();
  static ColData reflective_black();
  static ColData mirror();
  static ColData mirror_blue();
  static ColData mirror_green();
  static ColData mirror_purple();
  static ColData gold_flat();
  static ColData dark_purple_flat();
  static ColData white_flat();
  static ColData my_back_wall();
};

class LightSource {
private:
  Eigen::Vector4d position;
  LightIntensity intensity;

public:
  LightSource(const Eigen::Vector4d& position, const LightIntensity& intensity);
  LightSource(const Eigen::Vector3d& position, const LightIntensity& intensity);
  LightSource();

  ~LightSource();

  const LightIntensity& rgb() const;
  const Eigen::Vector4d& pos() const;
};