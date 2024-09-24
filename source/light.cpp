#include <light.hpp>

LightIntensity::LightIntensity(std::array<float, NUM_COL> rgb): rgb(rgb) {
  for (unsigned i = 0; i < NUM_COL; i++) {
    if (rgb.at(i) < 0) rgb[i] = 0;
    if (rgb.at(i) > 1) rgb[i] = 1;
  }
}
LightIntensity::LightIntensity(float red, float green, float blue): LightIntensity(std::array<float, NUM_COL>{red, green, blue}) {}
LightIntensity::LightIntensity(): LightIntensity(0, 0, 0) {}

LightIntensity LightIntensity::white(){
  return LightIntensity(1, 1, 1);
}

LightIntensity LightIntensity::red(){
  return LightIntensity(1, 0, 0);
}

LightIntensity LightIntensity::blue(){
  return LightIntensity(0, 0, 1);
}

LightIntensity LightIntensity::green(){
  return LightIntensity(0, 1, 0);
}

LightIntensity LightIntensity::black(){
  return LightIntensity(0, 0, 0);
}

LightIntensity LightIntensity::purple(){
  return LightIntensity(102.0 / 255.0, 51.0 / 255.0, 153.0 / 255.0);
}

LightIntensity LightIntensity::gold(){
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

ColData ColData::shiny(){
   return ColData(LightIntensity::white(), LightIntensity::white(), LightIntensity::white(), LightIntensity::white(), LightIntensity::white(), 1);
 }

ColData ColData::red_flat(){
  return ColData(LightIntensity::red(), LightIntensity::red(), LightIntensity::red(), LightIntensity::red(), LightIntensity::black(), 2);
}

ColData ColData::reflective_black(){
  return ColData(LightIntensity::black(), LightIntensity::black(), LightIntensity::white() * 0.2, LightIntensity::white() * 0.2, LightIntensity::black(), 3);
}

ColData ColData::mirror(){
  return ColData(LightIntensity::black(), LightIntensity::black(), LightIntensity::white(), LightIntensity::white(), LightIntensity::black(), 4);
}

ColData ColData::mirror_blue(){
  return ColData(LightIntensity::black(), LightIntensity::black(), LightIntensity::blue(), LightIntensity::blue(), LightIntensity::black(), 4);
}

ColData ColData::mirror_green(){
  return ColData(LightIntensity::green(), LightIntensity::green(), LightIntensity::green(), LightIntensity::green(), LightIntensity::black(), 4);
}

ColData ColData::mirror_purple(){
  return ColData(LightIntensity::black(), LightIntensity::black(), LightIntensity::purple(), LightIntensity::purple(), LightIntensity::black(), 4);
}

ColData ColData::gold_flat(){
  return ColData(LightIntensity::gold(), LightIntensity::gold(), LightIntensity::gold(), LightIntensity::gold(), LightIntensity::black(), 2);
}

ColData ColData::my_back_wall(){
  return ColData(LightIntensity::white(), LightIntensity::white() * 0.5, LightIntensity::white(), LightIntensity::white(), LightIntensity::blue(), 2);
}


LightSource::LightSource(
  const Eigen::Vector4f& position,
  const LightIntensity& intensity
): position(position), intensity(intensity)
{
  eigen_assert(position[3] == 1);
}

LightSource::LightSource(
  const Eigen::Vector3f& position,
  const LightIntensity& intensity
): LightSource((Eigen::Vector4f) position.homogeneous(), intensity) 
{}

LightSource::LightSource(): LightSource((Eigen::Vector3f) Eigen::Vector3f::Zero(), {0, 0, 0}) {}

LightSource::~LightSource() {
  std::cout << "Destructing Light Source at " << this << std::endl;
}

const LightIntensity& LightSource::rgb() const {
  return intensity;
}

const Eigen::Vector4f& LightSource::pos() const {
  return position;
}