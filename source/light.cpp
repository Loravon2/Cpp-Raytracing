#include <light.hpp>

LightIntensity::LightIntensity(std::array<float, NUM_COL> rgb): rgb(rgb) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    if (rgb.at(i) < 0) rgb[i] = 0;
    if (rgb.at(i) > 1) rgb[i] = 1;
  }
}
LightIntensity::LightIntensity(float red, float green, float blue): LightIntensity(std::array<float, NUM_COL>{red, green, blue}) {}
LightIntensity::LightIntensity(): LightIntensity(0, 0, 0) {}

inline LightIntensity LightIntensity::black() {
  return LightIntensity(0, 0, 0);
}
inline LightIntensity LightIntensity::silver() {
  return LightIntensity(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
}
inline LightIntensity LightIntensity::gray() {
  return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
}
inline LightIntensity LightIntensity::white() {
  return LightIntensity(1, 1, 1);
}
inline LightIntensity LightIntensity::maroon() {
  return LightIntensity(128.0 / 255.0, 0, 0);
}
inline LightIntensity LightIntensity::red() {
  return LightIntensity(1, 0, 0);
}
inline LightIntensity LightIntensity::purple() {
  return LightIntensity(128.0 / 255.0, 0, 128.0 / 255.0);
}
inline LightIntensity LightIntensity::fuchsia() {
  return LightIntensity(1, 0, 1);
}
inline LightIntensity LightIntensity::green() {
  return LightIntensity(0, 128.0 / 255.0, 0);
}
inline LightIntensity LightIntensity::lime() {
  return LightIntensity(0, 1, 0);
}
inline LightIntensity LightIntensity::olive() {
  return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 0);
}
inline LightIntensity LightIntensity::yellow() {
  return LightIntensity(1, 1, 0);
}
inline LightIntensity LightIntensity::navy() {
  return LightIntensity(0, 0, 128.0 / 255.0);
}
inline LightIntensity LightIntensity::blue() {
  return LightIntensity(0, 0, 1);
}
inline LightIntensity LightIntensity::teal() {
  return LightIntensity(0, 128.0 / 255.0, 128.0 / 255.0);
}
inline LightIntensity LightIntensity::aqua() {
  return LightIntensity(0, 1, 1);
}
inline LightIntensity LightIntensity::gold() {
  return LightIntensity(170.0 / 255.0, 127.0 / 255.0, 46.0 / 255.0);
}

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
    std::cout << "\nDestructing Light Source at " << this << std::endl;
  #endif
}

const LightIntensity& LightSource::rgb() const {
  return intensity;
}

const Eigen::Vector4d& LightSource::pos() const {
  return position;
}