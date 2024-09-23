#pragma once
#include <objects.hpp>

class Composites: public BaseObject{
public:
  static Intersection* cube(ColData col, float index);
  static Union* cube_pattern(ColData col, float index);
  static Subtraction* triangle_isosceles(ColData col, float index);
  static Subtraction* triangle_equilateral(ColData col, float index);
};