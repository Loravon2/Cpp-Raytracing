#pragma once

#include <iostream>
#include <array>
#include <Eigen>

#include "defines.h"

class LightIntensity {
private:
  std::array<float, NUM_COL> rgb;

public:
  LightIntensity(std::array<float, NUM_COL> rgb);
  LightIntensity(float red, float green, float blue);
  LightIntensity();

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
  Eigen::Vector4f position;
  LightIntensity intensity;

public:
  LightSource(const Eigen::Vector4f& position, const LightIntensity& intensity);
  LightSource(const Eigen::Vector3f& position, const LightIntensity& intensity);
  LightSource();

  ~LightSource();

  const LightIntensity& rgb() const;
  const Eigen::Vector4f& pos() const;
};