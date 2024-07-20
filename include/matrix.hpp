#pragma once

#include <array>
#include <exception>
#include <cmath>

#include <vector4D.hpp>
#include "defines.h"

class TransformMatrix {
private:
  std::array <std::array <double, 4>, 4> content;
public:
  TransformMatrix(std::array <std::array <double, 4>, 4> mat);
  TransformMatrix(std::array <double, 4> a, std::array <double, 4> b, std::array <double, 4> c, std::array <double, 4> d);
  TransformMatrix();

  virtual void inverse();

  const double& at(size_t i, size_t j) const;
  double& at(size_t i, size_t j);
  const Vector4D at(size_t i) const;

  void operator*=(const TransformMatrix& other);
};

const Vector4D operator*(const TransformMatrix& A, const Vector4D& v);
const TransformMatrix operator*(TransformMatrix A, const TransformMatrix& B);

class ScalingMatrix: TransformMatrix {
private:
  double ax;
  double ay;
  double az;
public:
  ScalingMatrix(double ax, double ay, double az);
  ScalingMatrix();

  void inverse() override;
};

class TranslationMatrix: TransformMatrix {
private:
  double tx;
  double ty;
  double tz;
public:
  TranslationMatrix(double tx, double ty, double tz);
  TranslationMatrix();

  void inverse() override;
};

class RotationMatrix: TransformMatrix {
private:
  double theta;
  axis_t axis;
public:
  RotationMatrix(double theta, axis_t i);
  RotationMatrix();

  void inverse() override;
};