#include <ray.hpp>




// with exception and error handling:

Ray::Ray(Eigen::Vector4f S, Eigen::Vector4f d, float n): S(S), d(d.normalized()), n(n) {
  
  //correct S[3]
  try {
    if (S[3] != 1) {
        throw std::invalid_argument("The fourth coordinate of S must be 1");
    }
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << " - Correcting S[3] to 1." << std::endl;
    S[3] = 1;
    }

  try {
    if (d[3] != 0) {
        throw std::invalid_argument("The fourth coordinate of d must be 0");
    }
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << " - Correcting d[3] to 0." << std::endl;
    d[3] = 0;
    }
  
}


Ray::Ray(Eigen::Vector3f S, Eigen::Vector3f d): Ray((Eigen::Vector4f) S.homogeneous(), d.homogeneous() - Eigen::Vector4f::UnitW()) {}

Ray::Ray(float sx, float sy, float sz, float dx, float dy, float dz): Ray(Eigen::Vector3f(sx, sy, sz), Eigen::Vector3f(dx, dy, dz)) {}

Ray::Ray(): Ray(0, 0, 0, 1, 0, 0) {}

const Eigen::Vector4f& Ray::get_S() const {
  return S;
}

const Eigen::Vector4f& Ray::get_d() const {
  return d;
}


const Ray Ray::reflect(const Eigen::Vector4f& P, Eigen::Vector4f normal) const {
  eigen_assert(P[3] == 1);
  eigen_assert(normal[3] == 0);

  normal.normalize();
  return Ray(P, d - 2*(d.dot(normal)) * normal);
}

const Ray Ray::reflect(const Eigen::Vector3f& P, const Eigen::Vector3f& normal) const {
  Eigen::Vector4f normalH = normal.homogeneous();
  normalH[3] = 0;
  return reflect(P.homogeneous(), normalH);
}


const Ray Ray::refract(const Eigen::Vector4f& P, Eigen::Vector4f normal, float n1, float n2) {
  eigen_assert(P[3] == 1);
  eigen_assert(normal[3] == 0);

  normal.normalize();
  return Ray(P, n1 / n2 * (d - (d.dot(normal) + sqrtf((n2 / n1) * (n2 / n1) + d.dot(normal) * d.dot(normal) - 1)) * normal));
}

const Ray Ray::refract(const Eigen::Vector3f& P, const Eigen::Vector3f& normal, float n1, float n2) {
  Eigen::Vector4f normalH = normal.homogeneous();
  normalH[3] = 0;
  return refract(P.homogeneous(), normalH, n1, n2);
}