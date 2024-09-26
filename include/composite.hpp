#pragma once
#include <objects.hpp>

class Composites {
public:
  // static Subtraction* halfsphere(ColData col, float index);
  static BaseObject* Cube(ColData col, float index);
  static BaseObject* Cube_Pattern(ColData col, float index);
  static BaseObject* Triangle_Isosceles(ColData col, float index);
  static BaseObject* Triangle_Equilateral(ColData col, float index);
  static BaseObject* Triforce(ColData col, float index);
};