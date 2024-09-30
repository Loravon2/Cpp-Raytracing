#include <Dense>
#include <cassert>

#include <composite.hpp>
#include <light.hpp>
#include <objects.hpp>
#include <ray.hpp>
#include <scene.hpp>
#include <custom_exceptions.hpp>
#include <defines.h>

int main() {
  // ray tests
  Ray r1(Eigen::Vector3d(1, 1, 1), Eigen::Vector3d(0, 1, 0), 1);
  CUSTOM_ASSERT(abs(r1.index() - 1.0) < EPSILON);
  CUSTOM_ASSERT((r1.start_point() - Eigen::Vector4d(1, 1, 1, 1)).norm() < EPSILON);
  CUSTOM_ASSERT((r1.direction() - Eigen::Vector4d(0, 1, 0, 0)).norm() < EPSILON);

  Ray r2 = r1.reflect(Eigen::Vector3d(1, 2, 1), Eigen::Vector3d(0, 1, 0));
  CUSTOM_ASSERT(abs(r2.index() - 1.0) < EPSILON);
  CUSTOM_ASSERT((r2.start_point() - Eigen::Vector4d(1, 2, 1, 1)).norm() < EPSILON);
  CUSTOM_ASSERT((r2.direction() - Eigen::Vector4d(0, -1, 0, 0)).norm() < EPSILON);

  Ray r3 = r1.refract(Eigen::Vector3d(1, 2, 1), Eigen::Vector3d(sqrtf64(2) / 2, sqrtf64(2) / 2, 0), 1);
  CUSTOM_ASSERT(abs(r3.index() - 1.0) < EPSILON);
  CUSTOM_ASSERT((r3.start_point() - Eigen::Vector4d(1, 2, 1, 1)).norm() < EPSILON);
  CUSTOM_ASSERT((r3.direction() - Eigen::Vector4d(0, 1, 0, 0)).norm() < EPSILON);

  Ray r4 = -Ray(0, 0, 0, 1, 0, 0, 2);
  CUSTOM_ASSERT(abs(r4.index() - 2.0) < EPSILON);
  CUSTOM_ASSERT((r4.start_point() - Eigen::Vector4d(0, 0, 0, 1)).norm() < EPSILON);
  CUSTOM_ASSERT((r4.direction() - Eigen::Vector4d(-1, 0, 0, 0)).norm() < EPSILON);

  Eigen::Transform<double, 3, Eigen::Projective> transform = Eigen::Transform<double, 3, Eigen::Projective> (Eigen::Translation<double, 3>(0, -2, 0));
  Ray r5 = transform * Ray(Eigen::Vector4d(1, 0, 0, 1), Eigen::Vector4d(0, 1, 0, 0), 1.5);
  CUSTOM_ASSERT(abs(r5.index() - 1.5) < EPSILON);
  CUSTOM_ASSERT((r5.start_point() - Eigen::Vector4d(1, -2, 0, 1)).norm() < EPSILON);
  CUSTOM_ASSERT((r5.direction() - Eigen::Vector4d(0, 1, 0, 0)).norm() < EPSILON);

  // light tests
  LightIntensity li1(std::array<float, NUM_COL>({0.8, 1.0, 0.2}));
  CUSTOM_ASSERT(abs(li1.at(0) - 0.8) < EPSILON);
  CUSTOM_ASSERT(abs(li1.at(1) - 1.0) < EPSILON);
  CUSTOM_ASSERT(abs(li1.at(2) - 0.2) < EPSILON);

  LightIntensity li2 = LightIntensity::gray() + LightIntensity::fuchsia();
  CUSTOM_ASSERT(abs(li2.at(0) - 1) < EPSILON);
  CUSTOM_ASSERT(abs(li2.at(1) - 128.0 / 255.0) < EPSILON);
  CUSTOM_ASSERT(abs(li2.at(2) - 1) < EPSILON);

  LightIntensity li3 = LightIntensity::white() * LightIntensity::lime();
  CUSTOM_ASSERT(abs(li3.at(0) - 0) < EPSILON);
  CUSTOM_ASSERT(abs(li3.at(1) - 1) < EPSILON);
  CUSTOM_ASSERT(abs(li3.at(2) - 0) < EPSILON);

  LightIntensity li4 = 0.5 * LightIntensity(1, 0.5, 0);
  CUSTOM_ASSERT(abs(li4.at(0) - 0.5) < EPSILON);
  CUSTOM_ASSERT(abs(li4.at(1) - 0.25) < EPSILON);
  CUSTOM_ASSERT(abs(li4.at(2) - 0) < EPSILON);

  LightSource ls1(Eigen::Vector3d(0, 1, 0), LightIntensity::black());
  CUSTOM_ASSERT((ls1.pos() - Eigen::Vector4d(0, 1, 0, 1)).norm() < EPSILON);
  CUSTOM_ASSERT(abs(ls1.rgb().at(0) - 0) < EPSILON);
  CUSTOM_ASSERT(abs(ls1.rgb().at(1) - 0) < EPSILON);
  CUSTOM_ASSERT(abs(ls1.rgb().at(2) - 0) < EPSILON);

  return 0;
}