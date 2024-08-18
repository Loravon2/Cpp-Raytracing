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
  ColData({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1.0)
  {}

LightIntensity::LightIntensity(float red, float green, float blue): rgb({red,green,blue}) {}
LightIntensity::LightIntensity(): rgb({0,0,0}) {}

LightIntensity LightIntensity::operator+(const LightIntensity li) {
  LightIntensity newLightIntensity(
    (1 - (1 - this->rgb[0])*(1 - li.rgb[0])),
    (1 - (1 - this->rgb[1])*(1 - li.rgb[1])),
    (1 - (1 - this->rgb[2])*(1 - li.rgb[2]))
  );
  return newLightIntensity;
}