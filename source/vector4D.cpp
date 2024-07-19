#include <vector4D.hpp>

vector4D::vector4D(double x, double y, double z, bool type): vector3D({x, y, z}), vectorType(type) {}
vector4D::vector4D(): vector4D(0, 0, 0, 1) {}

void vector4D::operator+=(const vector4D& other) {
  if (this->pointVector() && other.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }
  
  this->x() += other.x();
  this->y() += other.y();
  this->z() += other.z();
}

void vector4D::operator-=(const vector4D& other) {
  if (this->directionVector() && other.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }

  this->x() -= other.x();
  this->y() -= other.y();
  this->z() -= other.z();
  this->vectorType -= other.vectorType;
}

const vector4D operator+ (vector4D u, const vector4D& v) {
  u += v;
  return u;
}

const vector4D operator- (vector4D u, const vector4D& v) {
  u -= v;
  return u;
}

double operator* (const vector4D& u, const vector4D& v) {
  if (u.pointVector() || v.pointVector()) {
    /* ERROR HANDLING, since this is bogus */
  }

  double p = u.x() * v.x() + u.y() + v.y() + u.z() * v.z();
  return p;
}

double vector4D::norm() const {
  return std::sqrt(*this * *this);
}

void vector4D::normalize() {
  double n = this->norm();

  if (-EPSILON < n && n < EPSILON) {
    /* ERROR HANDLING, weil normalisieren von Nullvektor ist Arsch */
  }

  this->x() /= n;
  this->y() /= n;
  this->z() /= n;
}

bool vector4D::unitary() const {
  return (-EPSILON < this->norm()) && (this->norm() < EPSILON);
}

bool vector4D::pointVector() const {
  return vectorType;
}

bool vector4D::directionVector() const {
  return !vectorType;
}

const double& vector4D::x() const {
  return vector3D.at(0);
}

double& vector4D::x() {
  return vector3D.at(0);
}

const double& vector4D::y() const {
  return vector3D.at(1);
}

double& vector4D::y() {
  return vector3D.at(1);
}

const double& vector4D::z() const {
  return vector3D.at(2);
}

double& vector4D::z() {
  return vector3D.at(2);
}