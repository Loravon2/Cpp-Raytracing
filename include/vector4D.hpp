#pragma once

#include <array>
#include <cmath>

#include <defines.h>

class Vector4D {
private:
  std::array <double, 3> vector3D;
  bool vectorType;
public:
  Vector4D(double x, double y, double z, bool type);
  Vector4D();

  void operator+=(const Vector4D& other);
  void operator-=(const Vector4D& other);

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

const Vector4D operator+ (Vector4D u, const Vector4D& v);
const Vector4D operator- (Vector4D u, const Vector4D& v);

double operator* (const Vector4D& u, const Vector4D& v);