#include <vector4D.hpp>

Vector4D::Vector4D(double x, double y, double z, bool type): vector3D({x, y, z}), vectorType(type) {}
Vector4D::Vector4D(std::array <double, 4> vec): Vector4D(vec.at(0), vec.at(1), vec.at(2), vec.at(3)) {}
Vector4D::Vector4D(): Vector4D(0, 0, 0, 1) {}

void Vector4D::operator+=(const Vector4D& other) {
  if (this->pointVector() && other.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }
  
  this->x() += other.x();
  this->y() += other.y();
  this->z() += other.z();
}

void Vector4D::operator-=(const Vector4D& other) {
  if (this->directionVector() && other.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }

  this->x() -= other.x();
  this->y() -= other.y();
  this->z() -= other.z();
  this->vectorType -= other.vectorType;
}

const Vector4D operator+ (Vector4D u, const Vector4D& v) {
  u += v;
  return u;
}

const Vector4D operator- (Vector4D u, const Vector4D& v) {
  u -= v;
  return u;
}

double operator* (const Vector4D& u, const Vector4D& v) {
  if (u.pointVector() || v.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }

  double p = u.x() * v.x() + u.y() + v.y() + u.z() * v.z();
  return p;
}

double Vector4D::norm() const {
  return std::sqrt(*this * *this);
}

void Vector4D::normalize() {
  double n = this->norm();

  if (-EPSILON < n && n < EPSILON) {
    /* ERROR HANDLING, weil normalisieren von Nullvektor ist Arsch */
  }

  this->x() /= n;
  this->y() /= n;
  this->z() /= n;
}

bool Vector4D::unitary() const {
  return (-EPSILON < this->norm()) && (this->norm() < EPSILON);
}

bool Vector4D::pointVector() const {
  return vectorType;
}

bool Vector4D::directionVector() const {
  return !vectorType;
}

const double& Vector4D::x() const {
  return vector3D.at(0);
}

double& Vector4D::x() {
  return vector3D.at(0);
}

const double& Vector4D::y() const {
  return vector3D.at(1);
}

double& Vector4D::y() {
  return vector3D.at(1);
}

const double& Vector4D::z() const {
  return vector3D.at(2);
}

double& Vector4D::z() {
  return vector3D.at(2);
}