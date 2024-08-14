#include <iostream>
#include <Dense>

#include <ray.hpp>
#include <objects.hpp>
#include "math.h"
#include "defines.h"

int main() {
  
  HalfSpace* H = new HalfSpace();
  Transformation* rot = Transformation::Rotation_X(H, M_PI_2);

  RootObject* R = new RootObject(rot);

  Eigen::Vector3f S(1, 1, 0);
  Eigen::Vector3f d(sqrtf(2)/2, -sqrtf(2)/2, 0);

  Ray r(S, d, 1.0);

  IntersectionPoint P;
  if (R->intersect(r, P)) {
    std::cout << P.point << std::endl;
  }
  else {
    std::cout << "NO INTERSECTION" << std::endl;
  }

  return 0;
}