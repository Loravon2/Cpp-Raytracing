#include <light.hpp>

LightIntensity::LightIntensity(std::array<float, NUM_COL> rgb): rgb(rgb) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    if (rgb.at(i) < 0) rgb[i] = 0;
    if (rgb.at(i) > 1) rgb[i] = 1;
  }
}
LightIntensity::LightIntensity(float red, float green, float blue): LightIntensity(std::array<float, NUM_COL>{red, green, blue}) {}
LightIntensity::LightIntensity(): LightIntensity(0, 0, 0) {}

float LightIntensity::at(unsigned k) {
  return rgb.at(k);
}

void LightIntensity::operator+=(const LightIntensity& other) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    this->rgb.at(i) = 1 - (1 - this->rgb.at(i)) * (1 - other.rgb.at(i));
  }
}
const LightIntensity operator+(LightIntensity l1, const LightIntensity& l2) {
  l1 += l2;
  return l1;
}

void LightIntensity::operator*=(const LightIntensity& other) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    this->rgb.at(i) *= other.rgb.at(i);
  }
}
const LightIntensity operator*(LightIntensity l1, const LightIntensity& l2) {
  l1 *= l2;
  return l1;
}

void LightIntensity::operator*=(float scalar) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    this->rgb.at(i) *= scalar;
  }
}
const LightIntensity operator*(LightIntensity li, float scalar) {
  li *= scalar;
  return li;
}
const LightIntensity operator*(float scalar, LightIntensity li) {
  return li * scalar;
}


std::ostream& operator<<(std::ostream& out, const LightIntensity& li) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    out << li.rgb.at(i) << " ";
  }

  return out;
}



ColData::ColData(
  LightIntensity ambient,
  LightIntensity diffuse, 
  LightIntensity specular, 
  LightIntensity reflected, 
  LightIntensity refracted, 
  float shininess
  ):
  ambient(ambient), diffuse(diffuse), specular(specular), reflected(reflected), refracted(refracted), shininess(shininess)
  {}

ColData::ColData():
  ColData(LightIntensity(), LightIntensity(), LightIntensity(), LightIntensity(), LightIntensity(), 0.0)
  {}


LightSource::LightSource(
  const Eigen::Vector4d& position,
  const LightIntensity& intensity
): position(position), intensity(intensity)
{
  eigen_assert(position[3] == 1);
}

LightSource::LightSource(
  const Eigen::Vector3d& position,
  const LightIntensity& intensity
): LightSource((Eigen::Vector4d) position.homogeneous(), intensity) 
{}

LightSource::LightSource(): LightSource((Eigen::Vector3d) Eigen::Vector3d::Zero(), {0, 0, 0}) {}

LightSource::~LightSource() {
  #ifdef DEBUG
    std::cout << "Destructing Light Source at " << this << std::endl;
  #endif
}

const LightIntensity& LightSource::rgb() const {
  return intensity;
}

const Eigen::Vector4d& LightSource::pos() const {
  return position;
}