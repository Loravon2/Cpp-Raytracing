#pragma once

#include <array>

#include <vector4D.hpp>
#include "defines.h"

class TransformMatrix {
private:
  std::array <std::array <double, 4>, 4> content;
public:
  TransformMatrix(std::array <std::array <double, 4>, 4> mat);
  TransformMatrix();

  virtual void inverse() const = 0;

  const double& at(size_t i, size_t j) const;
  double& at(size_t i, size_t j);

  void operator*=(const TransformMatrix& other);
};

const Vector4D operator*(const TransformMatrix& A, const Vector4D& v);

class ScalingMatrix: TransformMatrix {
private:
public:
  ScalingMatrix(double ax, double ay, double az);
  ScalingMatrix();

  void inverse() const override;
};

class TranslationMatrix: TransformMatrix {
private:
public:
  TranslationMatrix(double tx, double ty, double dz);
  TranslationMatrix();

  void inverse() const override;
};

class RotationMatrix: TransformMatrix {
private:
public:
  RotationMatrix(double theta, axis_t i);
  RotationMatrix();

  void inverse() const override;
};