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


bool BaseObject::intersect(const Ray& r, std::vector<IntersectionPoint>& dest) const {
  return intersect(r, (Eigen::Transform<float, 3, Eigen::Projective>) Eigen::DiagonalMatrix<float, 3>(1, 1, 1), dest);
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

BaseObject::~BaseObject() {}

RootObject::RootObject(BaseObject* child): child(child) {}

RootObject::~RootObject() {
  std::cout << "Destructing Root Object at " << this << std::endl;
  delete child;
}

bool RootObject::intersect(const Ray& r, IntersectionPoint* dest) const {
  std::vector<IntersectionPoint> intersection_points;

  child->intersect(r, intersection_points);

  if (intersection_points.empty()) return false;

  std::sort(intersection_points.begin(), intersection_points.end());

  if (dest != nullptr) {
    *dest = intersection_points.at(0);
  }

  return true;
}

bool RootObject::included(const Eigen::Vector4f& point) const {
  return child->included(point);
}



Primitive::Primitive(ColData col, float index):
  col(col), index(index) 
  {}

Primitive::Primitive(): 
  Primitive(ColData(), 1.0)
  {}

Primitive::~Primitive() {
  std::cout << "Destructing Primitive at " << this << std::endl;
}


Sphere::Sphere(ColData col, float index):
  Primitive(col, index)
  {}

Sphere::Sphere():
  Sphere(ColData(), 1.0)
  {}

bool Sphere::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;

  Eigen::Vector4f lot =  Eigen::Vector3f::Zero().homogeneous() - modified.start_point();
  float dot = modified.direction().dot(lot);
  float delta = 1 + dot*dot - lot.norm() * lot.norm();

  if (delta < 0) return false;

  bool found = false;

  std::array<float, 2> t_arr = {dot + sqrtf(delta), dot - sqrtf(delta)};
  for (float t : t_arr) {
    if (t > 0) {
      Eigen::Vector4f P = modified.start_point() + t * modified.direction();
      Eigen::Vector4f normal = P - Eigen::Vector3f::Zero().homogeneous();

      float refr_index = index;

      if (this->included(r.start_point(), inverse_transform)) {
        normal *= -1.0;
        refr_index = 1.0; //THIS IS WERE WE NEED TO FIND THE INDEX OF THE WRAPPING OBJECT
      }

      dest.push_back(IntersectionPoint(P, normal, col, refr_index, t));
      found = true;
    }
  }

  return found;
}

bool Sphere::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4f modified = inverse_transform * point;
  
  float dist = abs((modified - Eigen::Vector3f::Zero().homogeneous()).norm());
  return dist < 1;
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

  float t = normal.dot(Eigen::Vector3f::Zero().homogeneous() - modified.start_point()) / normal.dot(modified.direction());
  
  if (t <= 0) return false;

  Eigen::Vector4f P = modified.start_point() + t * modified.direction();

  if (this->included(r.start_point(), inverse_transform)) {
    dest.push_back(IntersectionPoint(P, normal, col, 1.0, t)); //THIS IS WERE WE NEED TO FIND THE INDEX OF THE WRAPPING OBJECT
  }
  else {
    dest.push_back(IntersectionPoint(P, -normal, col, index, t));
  }

  return true;
}

bool HalfSpace::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4f modified = inverse_transform * point;
  
  return normal.dot(Eigen::Vector3f::Zero().homogeneous() - modified) > 0;
}

Transformation* Transformation::Scaling(BaseObject* child, float ax, float ay, float az) {
  return new Transformation(child, Eigen::DiagonalMatrix<float, 3>(ax, ay, az));
}

Transformation* Transformation::Rotation_X(BaseObject* child, float alpha) {
  return new Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitX()));
}
Transformation* Transformation::Rotation_Y(BaseObject* child, float alpha) {
  return new Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitY()));
}
Transformation* Transformation::Rotation_Z(BaseObject* child, float alpha) {
  return new Transformation(child, Eigen::AngleAxis<float>(alpha, Eigen::Vector3f::UnitZ()));
}

Transformation* Transformation::Translation(BaseObject* child, float dx, float dy, float dz) {
  return new Transformation(child, Eigen::Translation<float, 3>(dx, dy, dz));
}

Transformation::Transformation(BaseObject* child, Eigen::DiagonalMatrix<float, 3> scaling): child(child), transformation(scaling), inverse(scaling.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::AngleAxis<float> rotation): child(child), transformation(rotation), inverse(rotation.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::Translation<float, 3> translation): child(child), transformation(translation), inverse(translation.inverse()) {}

Transformation::~Transformation() {
  std::cout << "Destructing Transformation at " << this << std::endl;
  delete child;
}

bool Transformation::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Eigen::Transform<float, 3, Eigen::Projective> new_inverse_transform = inverse * inverse_transform;

  std::vector<IntersectionPoint> intersection_points;

  bool found = child->intersect(r, new_inverse_transform, intersection_points);

  for (IntersectionPoint& p : intersection_points) {
    dest.push_back(transformation * p);
  }

  return found;
}

bool Transformation::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Transform<float, 3, Eigen::Projective> new_inverse = inverse * inverse_transform;

  return child->included(point, new_inverse);
}

const Eigen::Transform<float, 3, Eigen::Projective>& Transformation::matrix() const {
  return transformation;
}

const Eigen::Transform<float, 3, Eigen::Projective>& Transformation::inverse_matrix() const {
  return inverse;
}



Combination::Combination(std::vector<BaseObject*> objects): objects(objects) {}

Combination::~Combination() {
  std::cout << "Destructing Combination at " << this << std::endl;
  for (BaseObject* O : objects) {
    delete O;
  }
}

Union::Union(std::vector<BaseObject*> objects): Combination(objects) {}

bool Union::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = false;
  for (BaseObject* O : objects) {
    bool foundO = O->intersect(r, inverse_transform, dest);
    found = found || foundO;
  }

  return found;
}

bool Union::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  for (BaseObject* O : objects) {
    if (O->included(point, inverse_transform)) {
      return true;
    }
  }

  return false;  
}


Intersection::Intersection(std::vector<BaseObject*> objects): Combination(objects) {}

bool Intersection::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = false;

  for (BaseObject* O1 : objects) {
    std::vector<IntersectionPoint> O1_points;
    O1->intersect(r, inverse_transform, O1_points);

    for (IntersectionPoint& p : O1_points) {
      bool available = true;

      for (BaseObject* O2 : objects) {
        if(O1 == O2) {
          continue;
        }

        if (!O2->included(p.point, inverse_transform)) {
          available = false;
          break;
        }
      }

      if (available) {
        dest.push_back(p);
        found = true;
      }
    }
  }

  return found;
}

bool Intersection::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  if (objects.empty()) {
    return false;
  }
  
  for (BaseObject* O : objects) {
    if (!O->included(point, inverse_transform)) {
      return false;
    }
  }

  return true;
}


Exclusion::Exclusion(std::vector<BaseObject*> objects): Combination(objects) {}

bool Exclusion::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = false;
  
  for (BaseObject* O1 : objects) {
    std::vector<IntersectionPoint> points;
    O1->intersect(r, points);

    for (IntersectionPoint& p : points) {
      bool available = true;
      
      for (BaseObject* O2 : objects) {
        if (O1 == O2) {
          continue;
        }

        if (O2->included(p.point, inverse_transform)) {
          available = false;
          break;
        }
      }

      if (available) {
        dest.push_back(p);
        found = true;
      }
    }
  }

  return found;
}

bool Exclusion::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  bool inc = false;

  for (BaseObject* O : objects) {
    if (O->included(point, inverse_transform)) {
      if (not inc) {
        inc = true;
      }
      else {
        return false;
      }
    }
  }

  return inc;
}


Subtraction::Subtraction(std::vector<BaseObject*> objects): Combination(objects) {}

bool Subtraction::intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  if (objects.empty()) {
    return false;
  }

  bool found = false;
  std::vector<IntersectionPoint> O1_points;
  objects[0]->intersect(r, inverse_transform, O1_points);

  for (IntersectionPoint& p : O1_points) {
    bool available = true;
    
    for (BaseObject* O2 : objects) {
      if (objects[0] == O2) {
        continue;
      }

      if (O2->included(p.point, inverse_transform)) {
        available = false;
        break;
      }
    }

    if (available) {
      found = true;
      dest.push_back(p);
    }
  }

  for (BaseObject* O2 : objects) {
    if (objects[0] == O2) {
      continue;
    }

    std::vector<IntersectionPoint> O2_points;
    O2->intersect(r, inverse_transform, O2_points);

    for (IntersectionPoint p : O2_points) {
      if (objects[0]->included(p.point, inverse_transform)) {
        found = true;
        dest.push_back(p);
      }
    }
  }

  return found;
}

bool Subtraction::included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const {
  if (objects.empty()) {
    return false;
  }

  if (not objects[0]->included(point, inverse_transform)) {
    return false;
  }

  for (BaseObject* O2 : objects) {
    if (objects[0] == O2) {
      continue;
    }

    if (O2->included(point, inverse_transform)) {
      return false;
    }
  }

  return true;
}