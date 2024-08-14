#include <objects.hpp>

IntersectionPoint::IntersectionPoint(Eigen::Vector4f point, Eigen::Vector4f normal, ColData color, float index, float distance): point(point), normal(normal.normalized()), color(color), index(index), distance(distance) {
  eigen_assert(point[3] == 1);
  eigen_assert(normal[3] == 0);
}

IntersectionPoint::IntersectionPoint(Eigen::Vector3f point, Eigen::Vector3f normal, ColData color, float index, float distance): IntersectionPoint((Eigen::Vector4f) point.homogeneous(), normal.homogeneous() - Eigen::Vector4f::UnitW(), color, index, distance) {}

IntersectionPoint::IntersectionPoint(): IntersectionPoint(Eigen::Vector4f(0, 0, 0, 1), Eigen::Vector4f(1, 0, 0, 0), ColData(), 1.0, 0.0) {}

bool IntersectionPoint::operator<(const IntersectionPoint& other) const {
  return this->distance < other.distance;
}

const IntersectionPoint operator*(const Eigen::Transform<float, 3, Eigen::Projective>& transformation, const IntersectionPoint& p) {
  return IntersectionPoint(transformation * p.point, transformation * p.normal, p.color, p.index, p.distance);
}


bool BaseObject::included(const Eigen::Vector3f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  return included((Eigen::Vector4f) (point.homogeneous() - Eigen::Vector4f::UnitW()), inverse_transform);
}

bool BaseObject::included(const Eigen::Vector4f& point) {
  return included(point, (Eigen::Transform<float, 3, Eigen::Projective>) Eigen::DiagonalMatrix<float, 3>(1, 1, 1));
}

bool BaseObject::included(const Eigen::Vector3f& point) {
  return included(point, (Eigen::Transform<float, 3, Eigen::Projective>) Eigen::DiagonalMatrix<float, 3>(1, 1, 1));
}

Primitive::Primitive(ColData col, float index):
  col(col), index(index) 
  {}

Primitive::Primitive(): 
  Primitive(ColData(), 1.0)
  {}



Sphere::Sphere(ColData col, float index):
  Primitive(col, index)
  {}

Sphere::Sphere():
  Sphere(ColData(), 1.0)
  {}

bool Sphere::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;

  Eigen::Vector4f lot = Eigen::Vector3f::Zero().homogeneous() - modified.start_point();
  float dot = modified.direction().dot(lot);
  float delta = 1 + dot*dot - lot.norm() * lot.norm();

  if (delta < 0) return false;

  std::array<float, 2> t_arr = {dot + sqrtf(delta), dot - sqrtf(delta)};
  for (float t : t_arr) {
    if (t > 0) {
      Eigen::Vector4f P = modified.start_point() + t * modified.direction();
      dest.push_back(IntersectionPoint(P, P - Eigen::Vector3f::Zero().homogeneous(), col, index, t));
    }
  }

  return true;
}

bool Sphere::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4f modified = inverse_transform * point;
  
  float dist = (modified - Eigen::Vector3f::Zero().homogeneous()).norm();
  return dist <= 1 && dist >= -1;
}


HalfSpace::HalfSpace(ColData col, float index, Eigen::Vector4f normal):
  Primitive(col, index), normal(normal)
  {
    eigen_assert(this->normal[3] == 0);
    this->normal.normalize();
  }

HalfSpace::HalfSpace():
  HalfSpace(ColData(), 1.0, Eigen::Vector4f(0, 0, 1, 0))
  {}

bool HalfSpace::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;
  
  if (normal.dot(modified.direction()) == 0) return false;

  float t = normal.dot(modified.start_point() - Eigen::Vector3f::Zero().homogeneous()) / normal.dot(modified.direction());

  if (t > 0) return false;

  Eigen::Vector4f P = modified.start_point() - t * modified.direction();

  dest.push_back(IntersectionPoint(P, normal, col, index, -t));

  return true;
}

bool HalfSpace::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4f modified = inverse_transform * point;
  
  return normal.dot(modified - Eigen::Vector3f::Zero().homogeneous()) > 0;
}

Transformation Transformation::Scaling(BaseObject* child, float ax, float ay, float az) {
  return Transformation(child, Eigen::DiagonalMatrix<float, 3>(ax, ay, az));
}

Transformation Transformation::Rotation_X(BaseObject* child, float alpha) {
  return Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitX()));
}
Transformation Transformation::Rotation_Y(BaseObject* child, float alpha) {
  return Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitY()));
}
Transformation Transformation::Rotation_Z(BaseObject* child, float alpha) {
  return Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitZ()));
}

Transformation Transformation::Translation(BaseObject* child, float dx, float dy, float dz) {
  return Transformation(child, Eigen::Translation<float, 3>(dx, dy, dz));
}

Transformation::Transformation(BaseObject* child, Eigen::DiagonalMatrix<float, 3> scaling): child(child), transformation(scaling), inverse(scaling.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::AngleAxis<float> rotation): child(child), transformation(rotation), inverse(rotation.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::Translation<float, 3> translation): child(child), transformation(translation), inverse(translation.inverse()) {}

bool Transformation::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Eigen::Transform<float, 3, Eigen::Projective> new_inverse_transform = inverse * inverse_transform;

  bool found = child->intersect(r, new_inverse_transform, dest);

  for (IntersectionPoint& p : dest) {
    p = transformation * p;
  }

  return found;
}

bool Transformation::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Transform<float, 3, Eigen::Projective> new_inverse = inverse * inverse_transform;

  return child->included(point, new_inverse);
}


Combination::Combination(BaseObject* O1, BaseObject* O2): O1(O1), O2(O2) {}


Union::Union(BaseObject* O1, BaseObject* O2): Combination(O1, O2) {}

bool Union::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = O1->intersect(r, inverse_transform, dest);
  found = O2->intersect(r, inverse_transform, dest) || found;
  return found;
}

bool Union::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  return O1->included(point, inverse_transform) || O2->included(point, inverse_transform);
}


Intersection::Intersection(BaseObject* O1, BaseObject* O2): Combination(O1, O2) {}

bool Intersection::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  std::vector<IntersectionPoint> O1_points;
  O1->intersect(r, inverse_transform, O1_points);

  bool found = false;
  for (IntersectionPoint& p : O1_points) {
    if (O2->included(p.point, inverse_transform)) {
      dest.push_back(p);
      found = true;
    }
  }

  return found;
}

bool Intersection::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  return O1->included(point, inverse_transform) && O2->included(point, inverse_transform);
}


Exclusion::Exclusion(BaseObject* O1, BaseObject* O2): Combination(O1, O2) {}

bool Exclusion::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  std::vector<IntersectionPoint> O1_points, O2_points;
  O1->intersect(r, inverse_transform, O1_points);
  O2->intersect(r, inverse_transform, O2_points);

  std::sort(O1_points.begin(), O1_points.end());
  std::sort(O2_points.begin(), O2_points.end());

  auto i = O1_points.begin();
  auto j = O2_points.begin();

  bool found = false;

  while (i < O1_points.end() || j < O2_points.end()) {
    if (i >= O1_points.end() || (*j < *i)) {
      dest.push_back(*j);
      j++;
      found = true;
      continue;
    }

    if (j >= O2_points.end() || (*i < *j)) {
      dest.push_back(*i);
      i++;
      found = true;
      continue;
    }

    i++;
    j++; //if both are equal, we dont want to add neither, since we are looking for the symmetric difference
  }

  return found;
}

bool Exclusion::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  return O1->included(point, inverse_transform) != O2->included(point, inverse_transform);
}


Subtraction::Subtraction(BaseObject* O1, BaseObject* O2): Combination(O1, O2) {}

bool Subtraction::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  std::vector<IntersectionPoint> O1_points, O2_points;
  O1->intersect(r, inverse_transform, O1_points);
  O2->intersect(r, inverse_transform, O2_points);

  std::sort(O1_points.begin(), O1_points.end());
  std::sort(O2_points.begin(), O2_points.end());

  auto i = O1_points.begin();
  auto j = O2_points.begin();

  bool found = false;

  while (i < O1_points.end()) {
    if (j >= O2_points.end() || *i < *j) {
      dest.push_back(*i);
      i++;
      found = true;
      continue;
    }

    if (*j < *i) {
      j++;
      continue;
    }

    i++; // we cant add i, since it is equal to j, i.e. subtracted
  }

  return found;
}

bool Subtraction::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  return O1->included(point, inverse_transform) && !O2->included(point, inverse_transform);
}