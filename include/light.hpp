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
  std::array<float, 3> rgb;

  public:
    LightIntensity(float red, float green, float blue);
    LightIntensity(std::array<float, 3> li);
    LightIntensity();
    LightIntensity operator+(const LightIntensity li);
    void print() { std::cout << "\n Value 1: " << this->rgb[0]<< "\n Value 2: " <<  this->rgb[1]<< "\n Value 3 " << this->rgb[2] << std::endl;}
};