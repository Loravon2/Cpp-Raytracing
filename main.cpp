#include <iostream>
#include <Dense>

#include <ray.hpp>
#include <objects.hpp>
#include <light.hpp>
#include "math.h"
#include "defines.h"

int main() {
  HalfSpace* H = new HalfSpace();
  Transformation* rot = Transformation::Rotation_X(H, -M_PI_2);

  Sphere* Sph = new Sphere();
  Transformation* trans = Transformation::Translation(Sph, 1, 1, 0);

  Union* U = new Union(rot, trans);

  RootObject* R = new RootObject(U);

  Eigen::Vector3f S(1, 1, 0);
  Eigen::Vector3f d(sqrtf(2)/2, -sqrtf(2)/2, 0);

  Ray r(S, d, 1.0);

  IntersectionPoint P;
  if (R->intersect(r, P)) {
    std::cout << P.point << "\n\n" << P.normal << "\n\n" << P.distance << std::endl;
  }
  else {
    std::cout << "NO INTERSECTION" << std::endl;
  }
  LightIntensity li(0.2,0.5,0.3);
  li.print();

  LightIntensity li2(0.7,0.12,0.5);

  LightIntensity li3 = li + li2;
  li3.print();
  return 0;
}