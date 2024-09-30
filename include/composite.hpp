#pragma once

#include <cmath>

#include <objects.hpp>

/**
 * \class Composites composite.hpp "include/composite.hpp"
 * 
 * \brief Helper class to construct composite objects.
 * 
 * You can use the static methods to construct composite objects.
 * The individual BaseObjects will be dynamical allocated and accordingly connected.
 * The returned BaseObject will act as a root of the composite and deleting it,
 * will free all allocated memory.
 */
class Composites {
public:
  /**
   * \brief Constructs a cube with uniform color and index.
   * 
   * The axis-aligned cube is centered at origin and has uniform side length of 1.
   * 
   * \param col color informations to be used
   * \param index refractive index to be used
   * 
   * \returns Intersection of six planes making a cube.
   */
  static BaseObject* Cube(ColData col, float index);

  /**
   * \brief Constructs a right regular triangular prism with uniform color and index.
   * 
   * The triangular faces are aligned to the z-axis,
   * i.e. the z-axis goes perpendicular through them.
   * The prism is centered at the origin and has uniform side length of 1.
   * 
   * \param col color informations to be used
   * \param index refractive index to be used
   * 
   * \returns Intersection of five planes making a prism.
   * 
   */
  static BaseObject* Prism(ColData col, float index);

  /**
   * \brief Constructs a Triforce with uniform color and index.
   * 
   * In reference to the video game series "The Legend of Zelda",
   * constructs a Triforce consisting of three triangular prisms,
   * representing Power, Wisdom and Courage.
   * 
   * The Triforce is centered at origin and each prism has uniform
   * side length of 1.
   * 
   * \param col color informations to be used
   * \param index refractive index to be used
   * 
   * \returns Union of three prisms making a Triforce.
   * 
   */
  static BaseObject* Triforce(ColData col, float index);
};