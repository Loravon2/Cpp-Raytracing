#pragma once

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