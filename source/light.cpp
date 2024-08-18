#include <light.hpp>

ColData::ColData(
  std::array<float, NUM_COL> ambient,
  std::array<float, NUM_COL> diffuse, 
  std::array<float, NUM_COL> specular, 
  std::array<float, NUM_COL> reflected, 
  std::array<float, NUM_COL> refracted, 
  float shininess
  ):
  ambient(ambient), diffuse(diffuse), specular(specular), reflected(reflected), refracted(refracted), shininess(shininess)
  {}

ColData::ColData():
  ColData({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0)
  {}

LightIntensity::LightIntensity(std::array<float, NUM_COL> rgb): rgb(rgb) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    if (rgb.at(i) < 0) rgb[i] = 0;
    if (rgb.at(i) > 1) rgb[i] = 1;
  }
}
LightIntensity::LightIntensity(float red, float green, float blue): LightIntensity(std::array<float, NUM_COL>{red, green, blue}) {}
LightIntensity::LightIntensity(): LightIntensity(0, 0, 0) {}

void LightIntensity::operator+=(const LightIntensity& other) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    this->rgb.at(i) = 1 - (1 - this->rgb.at(i)) * (1 - other.rgb.at(i));
  }
}

const LightIntensity operator+(LightIntensity l1, const LightIntensity& l2) {
  l1 += l2;
  return l1;
}

std::ostream& operator<<(std::ostream& out, const LightIntensity& li) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    out << li.rgb.at(i) << " ";
  }

  return out;
}