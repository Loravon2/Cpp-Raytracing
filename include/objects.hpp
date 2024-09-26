#pragma once
 
#include <iostream> //probably remove later 
#include <vector>
#include <tuple>
#include <array>
#include <Dense>

#include <ray.hpp>
#include <light.hpp>
#include "defines.h"


struct IntersectionPoint{
  Eigen::Vector4d point;
  Eigen::Vector4d normal;

  ColData color;
  float index;

  double distance;
  bool inside;

  IntersectionPoint(Eigen::Vector4d point, Eigen::Vector4d normal, ColData color, float index, double distance, bool inside);
  IntersectionPoint(Eigen::Vector3d point, Eigen::Vector3d normal, ColData color, float index, double distance, bool inside);
  IntersectionPoint();

  bool operator<(const IntersectionPoint& other) const;
};

const IntersectionPoint operator*(const Eigen::Transform<double, 3, Eigen::Projective>& transformation, const IntersectionPoint& p);


class BaseObject {
public:
  virtual ~BaseObject();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const = 0;
  bool intersect(const Ray& r, std::vector<IntersectionPoint>& dest) const;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const = 0;
  bool included(const Eigen::Vector3d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const;
  bool included(const Eigen::Vector4d& point);
  bool included(const Eigen::Vector3d& point);
};



class RootObject {
private:
  BaseObject* child;
public:
  RootObject(BaseObject* child);
  RootObject() = delete;

  ~RootObject();

  bool intersect(const Ray& r, IntersectionPoint* dest = nullptr) const;

  bool included(const Eigen::Vector4d& point) const;
};



class Primitive: public BaseObject {
protected:
  ColData col;
  float index;

public:
  Primitive(ColData col, float index);
  Primitive();

  virtual ~Primitive();
};

class Sphere: public Primitive {
public:
  Sphere(ColData col, float index);
  Sphere();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;
  
  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

class HalfSpace: public Primitive {
private:
  Eigen::Vector4d normal;
public:
  HalfSpace(ColData col, float index, Eigen::Vector4d normal);
  HalfSpace();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

class Cylinder: public Primitive {
public:
  Cylinder(ColData col, float index);
  Cylinder();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};


class Transformation: public BaseObject {
private:
  BaseObject* child; 

  Eigen::Transform<double, 3, Eigen::Projective> transformation;
  Eigen::Transform<double, 3, Eigen::Projective> inverse;

public:
  static Transformation* Scaling(BaseObject* child, double ax, double ay, double az);

  static Transformation* Rotation_X(BaseObject* child, double alpha);
  static Transformation* Rotation_Y(BaseObject* child, double alpha);
  static Transformation* Rotation_Z(BaseObject* child, double alpha);

  static Transformation* Translation(BaseObject* child, double dx, double dy, double dz);

  Transformation(BaseObject* child, Eigen::DiagonalMatrix<double, 3> scaling);
  Transformation(BaseObject* child, Eigen::AngleAxis<double> rotation);
  Transformation(BaseObject* child, Eigen::Translation<double, 3> translation);

  virtual ~Transformation();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;

  const Eigen::Transform<double, 3, Eigen::Projective>& matrix() const;
  const Eigen::Transform<double, 3, Eigen::Projective>& inverse_matrix() const;
};



class Combination: public BaseObject {
protected:
  std::vector<BaseObject*> objects;

public:
  Combination(std::vector<BaseObject*> objects);
  Combination() = delete;

  virtual ~Combination();
};

class Union: public Combination {
public:
  Union(std::vector<BaseObject*> objects);
  Union() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

class Intersection: public Combination {
public:
  Intersection(std::vector<BaseObject*> objects);
  Intersection() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

class Exclusion: public Combination {
public:
  Exclusion(std::vector<BaseObject*> objects);
  Exclusion() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

class Subtraction: public Combination {
public:
  Subtraction(std::vector<BaseObject*> objects);
  Subtraction() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};