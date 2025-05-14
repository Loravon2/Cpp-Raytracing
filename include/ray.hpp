#pragma once

#include <iostream>
#include <Dense>

#include <custom_exceptions.hpp>
#include "defines.h"

/**
 * \class Ray ray.hpp
 * 
 * \brief A Ray starting from a point and going straight into one direction.
 */
class Ray {
private:
  /**
   * \brief Starting point of the ray.
   * The fourth coordinate must be 1, because of homogenous coordinates.
   */
  Eigen::Vector4d S;
  /**
   * \brief Direction of the ray.
   * #d should always be normalized. The fourth coordinate must be 0, because of homogenous coordinates.
   */
  Eigen::Vector4d d;
  /**
   * \brief Refraction index of the medium that this Ray currently traverses.
   */
  float n;
public:
  /** 
   * \brief Base Constructor for Ray 
   */
  Ray(Eigen::Vector4d S, Eigen::Vector4d d, float n);
  /** 
   * \brief Constructor using three dimensional vectors.
   * Converts S into homogenous point and d into homogenous vector coordinates.
   */
  Ray(Eigen::Vector3d S, Eigen::Vector3d d, float n);
  /**
   * \brief Constructor taking each coefficient of #S and #d as a seperate argument.
   */
  Ray(double sx, double sy, double sz, double dx, double dy, double dz, float n);
  /** \brief Default Constructor for Ray */
  Ray();

  /**
   * \brief Getter function for the start point of this Ray.
   */
  const Eigen::Vector4d& start_point() const;
  /**
   * \brief Getter function for the direction vector of this Ray.
   */
  const Eigen::Vector4d& direction() const;
  /**
   * \brief Getter function for the refraction index of this Ray.
   */
  float index() const;

  /** 
   * \brief Calculates the reflection of this Ray at a point, on a surface with specific normal vector.
   * 
   * \param P Point that the Ray is reflected on. Becomes the new start point of the created Ray.
   * \param normal Normal vector of the surface, the reflection happens on.
   * 
   * \returns The reflected Ray.
   */
  const Ray reflect(const Eigen::Vector4d& P, Eigen::Vector4d normal) const;
  /**
   * \brief Calculates the reflection of this Ray at a point, on a surface with specific normal vector.
   * 
   * Converts P to homogenous point and normal to homogenous vector coordinates.
   */
  const Ray reflect(const Eigen::Vector3d& P, const Eigen::Vector3d& normal) const;

  /** 
   * \brief Calculates the refraction of this Ray at a point, through a surface with specific normal vector and refraction index.
   * 
   * \param P Point that the Ray is refracted through. Becomes the new start point of the created Ray.
   * \param normal Normal vector of the surface, the refraction happens on.
   * \param n2 Refraction index of the object, the Ray gets refracted into.
   * 
   * \returns The refracted Ray.
   */
  const Ray refract(const Eigen::Vector4d& P, Eigen::Vector4d normal, float n2) const;
  /**
   * \brief Calculates the refraction of this Ray at a point, through a surface with specific normal vector and refraction index.
   * 
   * Converts P to homogenous point and normal to homogenous vector coordinates.
   */
  const Ray refract(const Eigen::Vector3d& P, const Eigen::Vector3d& normal, float n2) const;

  /**
   * \brief Inverse of a Ray.
   * Flips #d (i.e. \f$d'=-d\f$) and leaves the rest of the Ray unchanged.
   * 
   * \returns Flipped Ray.
   */
  const Ray operator-() const;
};

/** 
 * \brief Applies a matrix on r.
 * Applies T to r.S and r.d
 * 
 * \param T Transformation matrix to be applied.
 * \param r Ray to be transformed.
 * 
 * \returns Transformed Ray.
 */
const Ray operator*(const Eigen::Transform<double, 3, Eigen::Projective>& T, const Ray& r);