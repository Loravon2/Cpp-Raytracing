#include <iostream>
#include <Dense>

#include "ray.hpp"
#include "defines.h"

int main() {
  //Eigen example
  Eigen::DiagonalMatrix<float, 3> A(4.5, 3.2, 1.1);
  Eigen::AngleAxis<float> B(2.1, Eigen::Vector3f::UnitX());

  Eigen::Transform<float, 3, Eigen::Projective> C = (Eigen::DiagonalMatrix<float, 3>) A.inverse().diagonal() * B.inverse();

  Eigen::Translation<float, 3> D(2.1, 1.2, 3.0);
  C *= D.inverse();

  //std::cout << C.matrix() << std::endl;

  //Ray example
  Eigen::Vector3f S(-1, 1, 1), P(0, 0, 0);
  Eigen::Vector3f d(1, -1, 0), normal(0, 1, 0);
  Ray r(S, d);

  std::cout << r.get_S() << "\n" << r.get_d() << "\n\n\n";

  Ray t = r.reflect(P, normal);

  std::cout << t.get_S() << "\n" << t.get_d() << std::endl;

  Ray f = r.refract(P, normal, 1.0, 2.3);

  std::cout << f.get_S() << "\n" << f.get_d() << std::endl; 

  return 0;
}