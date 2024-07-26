#include <iostream>
#include <Dense>

#include "defines.h"

int main() {
  Eigen::DiagonalMatrix<float, 3> A(4.5, 3.2, 1.1);
  Eigen::AngleAxis<float> B(2.1, Eigen::Vector3f::UnitX());

  Eigen::Transform<float, 3, Eigen::Projective> C = Eigen::DiagonalMatrix<float, 3>(A.inverse().diagonal()) * B.inverse();

  Eigen::Translation<float, 3> D(2.1, 1.2, 3.0);
  C *= D.inverse();

  std::cout << C.matrix() << std::endl;

  return 0;
}