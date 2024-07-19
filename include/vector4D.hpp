#pragma once

#include <array>
#include <cmath>

#include <defines.h>

class vector4D {
private:
  std::array <double, 3> vector3D;
  bool vectorType;
public:
  vector4D(double x, double y, double z, bool type);
  vector4D();

  void operator+=(const vector4D& other);
  void operator-=(const vector4D& other);

  double norm() const;

  void normalize();

  bool unitary() const;
  bool pointVector() const;
  bool directionVector() const;

  const double& x() const;
  double& x();
  const double& y() const;
  double& y();
  const double& z() const;
  double& z();
};

const vector4D operator+ (vector4D u, const vector4D& v);
const vector4D operator- (vector4D u, const vector4D& v);

double operator* (const vector4D& u, const vector4D& v);