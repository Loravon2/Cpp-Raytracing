#include <light.hpp>

ColData::ColData(std::array<float, NUM_COL> ambient, std::array<float, NUM_COL> diffuse, std::array<float, NUM_COL> specular, std::array<float, NUM_COL> reflected, std::array<float, NUM_COL> refracted, float shininess):
  ambient(ambient), diffuse(diffuse), specular(specular), reflected(reflected), refracted(refracted), shininess(shininess)
  {}

ColData::ColData():
  ColData({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0)
  {}