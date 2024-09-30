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

  inline static LightIntensity black() {
    return LightIntensity(0, 0, 0);
  }
  inline static LightIntensity silver() {
    return LightIntensity(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
  }
  inline static LightIntensity gray() {
    return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
  }
  inline static LightIntensity white() {
    return LightIntensity(1, 1, 1);
  }
  inline static LightIntensity maroon() {
    return LightIntensity(128.0 / 255.0, 0, 0);
  }
  inline static LightIntensity red() {
    return LightIntensity(1, 0, 0);
  }
  inline static LightIntensity purple() {
    return LightIntensity(128.0 / 255.0, 0, 128.0 / 255.0);
  }
  inline static LightIntensity fuchsia() {
    return LightIntensity(1, 0, 1);
  }
  inline static LightIntensity green() {
    return LightIntensity(0, 128.0 / 255.0, 0);
  }
  inline static LightIntensity lime() {
    return LightIntensity(0, 1, 0);
  }
  inline static LightIntensity olive() {
    return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 0);
  }
  inline static LightIntensity yellow() {
    return LightIntensity(1, 1, 0);
  }
  inline static LightIntensity navy() {
    return LightIntensity(0, 0, 128.0 / 255.0);
  }
  inline static LightIntensity blue() {
    return LightIntensity(0, 0, 1);
  }
  inline static LightIntensity teal() {
    return LightIntensity(0, 128.0 / 255.0, 128.0 / 255.0);
  }
  inline static LightIntensity aqua() {
    return LightIntensity(0, 1, 1);
  }
  inline static LightIntensity gold() {
    return LightIntensity(170.0 / 255.0, 127.0 / 255.0, 46.0 / 255.0);
  }

  const float& at(unsigned k) const;

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