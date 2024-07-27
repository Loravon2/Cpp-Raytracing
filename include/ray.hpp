#pragma once

#include <Dense>
#include <defines.h>

class Ray {
private:
  Eigen::Vector4f S;
  Eigen::Vector4f d;
public:
  Ray(Eigen::Vector3f S, Eigen::Vector3f d);
  Ray(Eigen::Vector4f S, Eigen::Vector4f d);
  Ray(float sx, float sy, float sz, float dx, float dy, float dz);
  Ray();

  const Eigen::Vector4f& get_S() const;
  const Eigen::Vector4f& get_d() const;

  const Ray reflect(const Eigen::Vector4f& P, Eigen::Vector4f normal) const;
  const Ray reflect(const Eigen::Vector3f& P, const Eigen::Vector3f& normal) const;

  const Ray refract(const Eigen::Vector4f& P, Eigen::Vector4f normal, float n1, float n2);
  const Ray refract(const Eigen::Vector3f& P, const Eigen::Vector3f& normal, float n1, float n2);
};