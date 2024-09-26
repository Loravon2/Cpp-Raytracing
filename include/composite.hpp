#pragma once
#include <objects.hpp>

class Composites: public BaseObject{
public:
  static BaseObject* cube(ColData col, float index);
};