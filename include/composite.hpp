#pragma once

#include <cmath>

#include <objects.hpp>

class Composites {
public:
  static BaseObject* Cube(ColData col, float index);
  static BaseObject* Prism(ColData col, float index);
  static BaseObject* Triforce(ColData col, float index);
};