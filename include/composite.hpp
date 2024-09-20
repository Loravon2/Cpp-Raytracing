#pragma once
#include <objects.hpp>

class Composites: public BaseObject{
public:
  static Intersection* cube(ColData col, float index);
  static BaseObject* triangle(ColData col, float index);
};