#include <ray.hpp>

Ray::Ray(Eigen::Vector4f S, Eigen::Vector4f d, float n): S(S), d(d.normalized()), n(n) {
  eigen_assert(S[3] == 1);
  eigen_assert(d[3] == 0); // could use warnings instead and correct mistake
}

Ray::Ray(Eigen::Vector3f S, Eigen::Vector3f d, float n): Ray((Eigen::Vector4f) S.homogeneous(), d.homogeneous() - Eigen::Vector4f::UnitW(), n) {}

Ray::Ray(float sx, float sy, float sz, float dx, float dy, float dz, float n): Ray(Eigen::Vector3f(sx, sy, sz), Eigen::Vector3f(dx, dy, dz), n) {}

Ray::Ray(): Ray(0, 0, 0, 1, 0, 0, 1) {}

const Eigen::Vector4f& Ray::start_point() const {
  return S;
}

const Eigen::Vector4f& Ray::direction() const {
  return d;
}

float Ray::index() const {
  return n;
}


const Ray Ray::reflect(const Eigen::Vector4f& P, Eigen::Vector4f normal) const {
  eigen_assert(P[3] == 1);
  eigen_assert(normal[3] == 0);

  normal.normalize();
  return Ray(P, d - 2*(d.dot(normal)) * normal, this->n);
}

const Ray Ray::reflect(const Eigen::Vector3f& P, const Eigen::Vector3f& normal) const {
  Eigen::Vector4f normalH = normal.homogeneous();
  normalH[3] = 0;
  return reflect(P.homogeneous(), normalH);
}


const Ray Ray::refract(const Eigen::Vector4f& P, Eigen::Vector4f normal, float n2) const {
  eigen_assert(P[3] == 1);
  eigen_assert(normal[3] == 0);

  normal.normalize();
  return Ray(P, n / n2 * (d - (d.dot(normal) + sqrtf((n2 / n) * (n2 / n) + d.dot(normal) * d.dot(normal) - 1)) * normal), n2);
}

const Ray Ray::refract(const Eigen::Vector3f& P, const Eigen::Vector3f& normal, float n2) const {
  Eigen::Vector4f normalH = normal.homogeneous();
  normalH[3] = 0;
  return refract(P.homogeneous(), normalH, n2);
}


const Ray Ray::operator-() const {
  Ray r(this->S, -this->d, this->n);

  return r;
}



const Ray operator*(const Eigen::Transform<float, 3, Eigen::Projective>& T, const Ray& r) {
  return Ray((Eigen::Vector4f) (T * r.start_point()), (Eigen::Vector4f) (T * r.direction()), r.index());
}