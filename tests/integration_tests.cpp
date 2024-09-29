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
  // interHalfSpace.md
  BaseObject* obj1 = new HalfSpace(ColData(), 1, Eigen::Vector4d(0, 1, 0, 0));
  RootObject* root = new RootObject(obj1);
  IntersectionPoint p;

  Ray r1(Eigen::Vector4d(-1, 1, 0, 1), Eigen::Vector4d(sqrtf64(2) / 2, - sqrtf64(2) / 2, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(0, 0, 0, 1)).norm() < EPSILON);

  r1 = Ray(Eigen::Vector4d(1, 1, 0, 1), Eigen::Vector4d(0, -1, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(1, 0, 0, 1)).norm() < EPSILON);

  r1 = Ray(Eigen::Vector4d(-1, 0, 0, 1), Eigen::Vector4d(1, 0, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(-1, 0, 0, 1)).norm() < EPSILON);

  delete root;

  
  // interModHalfSpace.md
  obj1 = new HalfSpace(ColData(), 1, Eigen::Vector4d(0, 1, 0, 0));
  BaseObject* obj2 = Transformation::Translation(obj1, 0, 1, 0);
  root = new RootObject(obj2);

  r1 = Ray(Eigen::Vector4d(1, 2, 0, 1), Eigen::Vector4d(0, -1, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(1, 1, 0, 1)).norm() < EPSILON);

  r1 = Ray(Eigen::Vector4d(-1, 2, 0, 1), Eigen::Vector4d(sqrtf64(2) / 2, - sqrtf64(2) / 2, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(0, 1, 0, 1)).norm() < EPSILON);

  delete root;


  // interSphere.md
  obj1 = new Sphere(ColData(), 1);
  root = new RootObject(obj1);

  r1 = Ray(Eigen::Vector4d(-3, 0, 0, 1), Eigen::Vector4d(1, 0, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(-1, 0, 0, 1)).norm() < EPSILON);

  r1 = Ray(Eigen::Vector4d(3, 3, 0, 1), Eigen::Vector4d(- sqrtf64(2) / 2, - sqrtf64(2) / 2, 0, 0), 1);
  root->intersect(r1, &p);
  CUSTOM_ASSERT((p.point - Eigen::Vector4d(sqrtf64(2) / 2, sqrtf64(2) / 2, 0, 1)).norm() < EPSILON);

  delete root;

  return 0;
}