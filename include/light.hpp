#pragma once

#include <iostream>
#include <array>

#include "defines.h"

struct ColData {
  std::array<float, NUM_COL> ambient;
  std::array<float, NUM_COL> diffuse;
  std::array<float, NUM_COL> specular;
  std::array<float, NUM_COL> reflected;
  std::array<float, NUM_COL> refracted;
  float shininess;

  ColData(std::array<float, NUM_COL> ambient, std::array<float, NUM_COL> diffuse, std::array<float, NUM_COL> specular, std::array<float, NUM_COL> reflected, std::array<float, NUM_COL> refracted, float shininess);
  ColData();
};

class LightIntensity {
private:
  std::array<float, NUM_COL> rgb;

public:
  LightIntensity(std::array<float, NUM_COL> rgb);
  LightIntensity(float red, float green, float blue);
  LightIntensity();

  void operator+=(const LightIntensity& l2);
  friend const LightIntensity operator+(LightIntensity l1, const LightIntensity& l2);
  
  friend std::ostream& operator<<(std::ostream& out, const LightIntensity& li);
};