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

  inline static LightIntensity black();
  inline static LightIntensity silver();
  inline static LightIntensity gray();
  inline static LightIntensity white();
  inline static LightIntensity maroon();
  inline static LightIntensity red();
  inline static LightIntensity purple();
  inline static LightIntensity fuchsia();
  inline static LightIntensity green();
  inline static LightIntensity lime();
  inline static LightIntensity olive();
  inline static LightIntensity yellow();
  inline static LightIntensity navy();
  inline static LightIntensity blue();
  inline static LightIntensity teal();
  inline static LightIntensity aqua();
  inline static LightIntensity gold();

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