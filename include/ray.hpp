#pragma once

#include <iostream>

#include <Dense>
#include "defines.h"

class Ray {
private:
  Eigen::Vector4d S;
  Eigen::Vector4d d;
  float n;
public:
  Ray(Eigen::Vector3d S, Eigen::Vector3d d, float n);
  Ray(Eigen::Vector4d S, Eigen::Vector4d d, float n);
  Ray(double sx, double sy, double sz, double dx, double dy, double dz, float n);
  Ray();

  const Eigen::Vector4d& start_point() const;
  const Eigen::Vector4d& direction() const;
  float index() const;

  const Ray reflect(const Eigen::Vector4d& P, Eigen::Vector4d normal) const;
  const Ray reflect(const Eigen::Vector3d& P, const Eigen::Vector3d& normal) const;

  const Ray refract(const Eigen::Vector4d& P, Eigen::Vector4d normal, float n2) const;
  const Ray refract(const Eigen::Vector3d& P, const Eigen::Vector3d& normal, float n2) const;

  const Ray operator-() const;
};

const Ray operator*(const Eigen::Transform<double, 3, Eigen::Projective>& T, const Ray& r);