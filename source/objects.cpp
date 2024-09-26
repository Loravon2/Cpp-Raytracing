#include <objects.hpp>

IntersectionPoint::IntersectionPoint(Eigen::Vector4d point, Eigen::Vector4d normal, ColData color, float index, double distance, bool inside): point(point), normal(normal.normalized()), color(color), index(index), distance(distance), inside(inside) {
  eigen_assert(point[3] == 1);
  eigen_assert(normal[3] == 0);
}

IntersectionPoint::IntersectionPoint(Eigen::Vector3d point, Eigen::Vector3d normal, ColData color, float index, double distance, bool inside): IntersectionPoint((Eigen::Vector4d) point.homogeneous(), normal.homogeneous() - Eigen::Vector4d::UnitW(), color, index, distance, inside) {}

IntersectionPoint::IntersectionPoint(): IntersectionPoint(Eigen::Vector4d(0, 0, 0, 1), Eigen::Vector4d(1, 0, 0, 0), ColData(), 1.0, 0.0, false) {}

bool IntersectionPoint::operator<(const IntersectionPoint& other) const {
  return this->distance < other.distance;
}

const IntersectionPoint operator*(const Eigen::Transform<double, 3, Eigen::Projective>& transformation, const IntersectionPoint& p) {
  return IntersectionPoint(transformation * p.point, transformation * p.normal, p.color, p.index, p.distance, p.inside);
}


bool BaseObject::intersect(const Ray& r, std::vector<IntersectionPoint>& dest) const {
  return intersect(r, (Eigen::Transform<double, 3, Eigen::Projective>) Eigen::DiagonalMatrix<double, 3>(1, 1, 1), dest);
}

bool BaseObject::included(const Eigen::Vector3d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  return included((Eigen::Vector4d) (point.homogeneous() - Eigen::Vector4d::UnitW()), inverse_transform);
}

bool BaseObject::included(const Eigen::Vector4d& point) {
  return included(point, (Eigen::Transform<double, 3, Eigen::Projective>) Eigen::DiagonalMatrix<double, 3>(1, 1, 1));
}

bool BaseObject::included(const Eigen::Vector3d& point) {
  return included(point, (Eigen::Transform<double, 3, Eigen::Projective>) Eigen::DiagonalMatrix<double, 3>(1, 1, 1));
}

BaseObject::~BaseObject() {}

RootObject::RootObject(BaseObject* child): child(child) {}

RootObject::~RootObject() {
  #ifdef DEBUG
    std::cout << "Destructing Root Object at " << this << std::endl;
  #endif

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

bool RootObject::included(const Eigen::Vector4d& point) const {
  return child->included(point);
}



Primitive::Primitive(ColData col, float index):
  col(col), index(index) 
  {}

Primitive::Primitive(): 
  Primitive(ColData(), 1.0)
  {}

Primitive::~Primitive() {
  #ifdef DEBUG
    std::cout << "Destructing Primitive at " << this << std::endl;
  #endif
}


Sphere::Sphere(ColData col, float index):
  Primitive(col, index)
  {}

Sphere::Sphere():
  Sphere(ColData(), 1.0)
  {}

bool Sphere::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;

  Eigen::Vector4d lot =  Eigen::Vector3d::Zero().homogeneous() - modified.start_point();
  double dot = modified.direction().dot(lot);
  double delta = 1 + dot*dot - lot.norm() * lot.norm();

  if (delta < 0) return false;

  bool found = false;

  std::array<double, 2> t_arr = {dot + sqrtf64(delta), dot - sqrtf64(delta)};
  for (double t : t_arr) {
    if (t > 0) {
      Eigen::Vector4d P = modified.start_point() + t * modified.direction();
      Eigen::Vector4d normal = P - Eigen::Vector3d::Zero().homogeneous();

      bool inside = false;
      if (normal.dot(modified.direction()) > 0) {
        inside = true;
      }

      dest.push_back(IntersectionPoint(P, normal, col, index, t, inside));
      found = true;
    }
  }

  return found;
}

bool Sphere::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4d modified = inverse_transform * point;
  
  double dist = abs((modified - Eigen::Vector3d::Zero().homogeneous()).norm());
  return dist < 1;
}


HalfSpace::HalfSpace(ColData col, float index, Eigen::Vector4d normal):
  Primitive(col, index), normal(normal)
  {
    eigen_assert(this->normal[3] == 0);
    this->normal.normalize();
  }

HalfSpace::HalfSpace():
  HalfSpace(ColData(), 1.0, Eigen::Vector4d(0, 0, 1, 0))
  {}

bool HalfSpace::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;
  
  if (normal.dot(modified.direction()) == 0) return false;

<<<<<<< HEAD
  double t = normal.dot(Eigen::Vector3d::Zero().homogeneous() - modified.start_point()) / normal.dot(modified.direction());
  
  if (t <= 0) return false;

  Eigen::Vector4d P = modified.start_point() + t * modified.direction();

  bool inside = false;
  if (this->normal.dot(modified.direction()) > 0) {
    inside = true;
=======
  float t = normal.dot(Eigen::Vector3f::Zero().homogeneous() - modified.start_point()) / normal.dot(modified.direction());
  
  if (t <= 0) return false;

  Eigen::Vector4f P = modified.start_point() + t * modified.direction();

  if (this->included(r.start_point(), inverse_transform)) {
    dest.push_back(IntersectionPoint(P, normal, col, 1.0, t)); //THIS IS WERE WE NEED TO FIND THE INDEX OF THE WRAPPING OBJECT
  }
  else {
    dest.push_back(IntersectionPoint(P, -normal, col, index, t));
>>>>>>> 07365f0 (made composite for cube)
  }

  dest.push_back(IntersectionPoint(P, normal, col, index, t, inside));
  
  return true;
}

bool HalfSpace::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4d modified = inverse_transform * point;
  
<<<<<<< HEAD
  return normal.dot(Eigen::Vector3d::Zero().homogeneous() - modified) > 0;
=======
  return normal.dot(Eigen::Vector3f::Zero().homogeneous() - modified) > 0;
>>>>>>> 07365f0 (made composite for cube)
}

Cylinder::Cylinder(ColData col, float index):
  Primitive(col, index)
  {}

Cylinder::Cylinder(): 
  Cylinder(ColData(), 1.0)
  {}

bool Cylinder::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Ray modified = inverse_transform * r;

  Eigen::Vector2d projected_P(modified.start_point()[0], modified.start_point()[1]);
  Eigen::Vector2d projected_d(modified.direction()[0], modified.direction()[1]);

  if ((projected_d.norm() >= 0 - EPSILON) and (projected_d.norm() <= 0 + EPSILON)) {
    return false;
  }

  double term1 = projected_P.dot(projected_d) / (projected_d.norm() * projected_d.norm());
  double term2 = (projected_P.norm() * projected_P.norm() - 1) / (projected_d.norm() * projected_d.norm());

  if ((term1 * term1 - term2) < 0 + EPSILON) {
    return false;
  }

  bool found = false;

  std::array<double, 2> t_arr = {-term1 - sqrtf64(term1 * term1 - term2), -term1 + sqrtf64(term1 * term1 - term2)};
  
  for (double t : t_arr) {
    if (t < 0) {
      continue;
    }

    Eigen::Vector4d P = modified.start_point() + t * modified.direction();
    Eigen::Vector4d normal(P[0], P[1], 0, 0);

    bool inside = false;
    if (normal.dot(modified.direction()) > 0) {
      inside = true;
    }

    dest.push_back(IntersectionPoint(P, normal, col, index, t, inside));
    found = true;
  }

  return found;
}

bool Cylinder::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Vector4d modified = inverse_transform * point;

  return ((modified[0] * modified[0] + modified[1] * modified[1]) < 1); 
}


Transformation* Transformation::Scaling(BaseObject* child, double ax, double ay, double az) {
  return new Transformation(child, Eigen::DiagonalMatrix<double, 3>(ax, ay, az));
}

Transformation* Transformation::Rotation_X(BaseObject* child, double alpha) {
  return new Transformation(child, Eigen::AngleAxis<double>(alpha, Eigen::Vector3d::UnitX()));
}
Transformation* Transformation::Rotation_Y(BaseObject* child, double alpha) {
  return new Transformation(child, Eigen::AngleAxis<double>(alpha, Eigen::Vector3d::UnitY()));
}
Transformation* Transformation::Rotation_Z(BaseObject* child, double alpha) {
  return new Transformation(child, Eigen::AngleAxis<double>(alpha, Eigen::Vector3d::UnitZ()));
}

Transformation* Transformation::Translation(BaseObject* child, double dx, double dy, double dz) {
  return new Transformation(child, Eigen::Translation<double, 3>(dx, dy, dz));
}

Transformation::Transformation(BaseObject* child, Eigen::DiagonalMatrix<double, 3> scaling): child(child), transformation(scaling), inverse(scaling.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::AngleAxis<double> rotation): child(child), transformation(rotation), inverse(rotation.inverse()) {}
Transformation::Transformation(BaseObject* child, Eigen::Translation<double, 3> translation): child(child), transformation(translation), inverse(translation.inverse()) {}

Transformation::~Transformation() {
  #ifdef DEBUG
    std::cout << "Destructing Transformation at " << this << std::endl;
  #endif
  
  delete child;
}

bool Transformation::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  Eigen::Transform<double, 3, Eigen::Projective> new_inverse_transform = inverse * inverse_transform;

  std::vector<IntersectionPoint> intersection_points;

  bool found = child->intersect(r, new_inverse_transform, intersection_points);

  for (IntersectionPoint& p : intersection_points) {
    dest.push_back(transformation * p);
  }

  return found;
}

bool Transformation::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  Eigen::Transform<double, 3, Eigen::Projective> new_inverse = inverse * inverse_transform;

  return child->included(point, new_inverse);
}

const Eigen::Transform<double, 3, Eigen::Projective>& Transformation::matrix() const {
  return transformation;
}

const Eigen::Transform<double, 3, Eigen::Projective>& Transformation::inverse_matrix() const {
  return inverse;
}



Combination::Combination(std::vector<BaseObject*> objects): objects(objects) {}

Combination::~Combination() {
  #ifdef DEBUG
    std::cout << "Destructing Combination at " << this << std::endl;
  #endif
  
  for (BaseObject* O : objects) {
    delete O;
  }
}

Union::Union(std::vector<BaseObject*> objects): Combination(objects) {}

bool Union::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = false;
  for (BaseObject* O : objects) {
    bool foundO = O->intersect(r, inverse_transform, dest);
    found = found || foundO;
  }

  return found;
}

bool Union::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
  for (BaseObject* O : objects) {
    if (O->included(point, inverse_transform)) {
      return true;
    }
  }

  return false;  
}


Intersection::Intersection(std::vector<BaseObject*> objects): Combination(objects) {}

bool Intersection::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
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

<<<<<<< HEAD
        if (!O2->included(p.point)) {
=======
        if (!O2->included(p.point, inverse_transform)) {
>>>>>>> 07365f0 (made composite for cube)
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

bool Intersection::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
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

bool Exclusion::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
  bool found = false;
  
  for (BaseObject* O1 : objects) {
    std::vector<IntersectionPoint> points;
    O1->intersect(r, inverse_transform, points);

    for (IntersectionPoint& p : points) {
      unsigned inclusions = 0;
      
      for (BaseObject* O2 : objects) {
        if (O1 == O2) {
          continue;
        }

        if (O2->included(p.point)) {
          inclusions++;
        }
      }

      if (inclusions <= 1) {
        dest.push_back(p);
        found = true;
      }
    }
  }

  return found;
}

bool Exclusion::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
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

bool Subtraction::intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const {
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

      if (O2->included(p.point)) {
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

    for (IntersectionPoint& p : O2_points) {
      if (objects[0]->included(p.point)) {
        found = true;
        dest.push_back(p);
      }
    }
  }

  return found;
}

bool Subtraction::included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const {
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