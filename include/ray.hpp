#pragma once

#include <Dense>
#include "defines.h"

class Ray {
private:
  Eigen::Vector4f S;
  Eigen::Vector4f d;
  float n;
public:
  Ray(Eigen::Vector3f S, Eigen::Vector3f d, float n);
  Ray(Eigen::Vector4f S, Eigen::Vector4f d, float n);
  Ray(float sx, float sy, float sz, float dx, float dy, float dz, float n);
  Ray();

  const Eigen::Vector4f& start_point() const;
  const Eigen::Vector4f& direction() const;
  float index() const;

  const Ray reflect(const Eigen::Vector4f& P, Eigen::Vector4f normal) const;
  const Ray reflect(const Eigen::Vector3f& P, const Eigen::Vector3f& normal) const;

  const Ray refract(const Eigen::Vector4f& P, Eigen::Vector4f normal, float n2);
  const Ray refract(const Eigen::Vector3f& P, const Eigen::Vector3f& normal, float n2);
};

const Ray operator*(const Eigen::Transform<float, 3, Eigen::Projective>& T, const Ray& r);