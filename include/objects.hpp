#pragma once
 
#include <vector>
#include <tuple>
#include <array>
#include <Dense>

#include <ray.hpp>
#include <light.hpp>
#include "defines.h"


struct IntersectionPoint{
  Eigen::Vector4f point;
  Eigen::Vector4f normal;

  ColData color;
  float index;

  float distance;

  IntersectionPoint(Eigen::Vector4f point, Eigen::Vector4f normal, ColData color, float index, float distance);
  IntersectionPoint(Eigen::Vector3f point, Eigen::Vector3f normal, ColData color, float index, float distance);
  IntersectionPoint();

  bool operator<(const IntersectionPoint& other) const;
};

const IntersectionPoint operator*(const Eigen::Transform<float, 3, Eigen::Projective>& transformation, const IntersectionPoint& p);


class BaseObject {
public:
  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const = 0;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const = 0;
  bool included(const Eigen::Vector3f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const;
  bool included(const Eigen::Vector4f& point);
  bool included(const Eigen::Vector3f& point);
};



class Primitive: public BaseObject {
protected:
  ColData col;
  float index;

public:
  Primitive(ColData col, float index);
  Primitive();
};

class Sphere: public Primitive {
public:
  Sphere(ColData col, float index);
  Sphere();

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;
  
  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};

class HalfSpace: public Primitive {
private:
  Eigen::Vector4f normal;
public:
  HalfSpace(ColData col, float index, Eigen::Vector4f normal);
  HalfSpace();

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};

//space for more primitives :}


class Transformation: public BaseObject {
private:
  BaseObject* child; 

  Eigen::Transform<float, 3, Eigen::Projective> transformation;
  Eigen::Transform<float, 3, Eigen::Projective> inverse;

public:
  static Transformation Scaling(BaseObject* child, float ax, float ay, float az);

  static Transformation Rotation_X(BaseObject* child, float alpha);
  static Transformation Rotation_Y(BaseObject* child, float alpha);
  static Transformation Rotation_Z(BaseObject* child, float alpha);

  static Transformation Translation(BaseObject* child, float dx, float dy, float dz);

  Transformation(BaseObject* child, Eigen::DiagonalMatrix<float, 3> scaling);
  Transformation(BaseObject* child, Eigen::AngleAxis<float> rotation);
  Transformation(BaseObject* child, Eigen::Translation<float, 3> translation);

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};



class Combination: public BaseObject {
protected:
  BaseObject* O1;
  BaseObject* O2;

public:
  Combination(BaseObject* O1, BaseObject* O2);
  Combination() = delete;
};

class Union: public Combination {
public:
  Union(BaseObject* O1, BaseObject* O2);
  Union() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};

class Intersection: public Combination {
  Intersection(BaseObject* O1, BaseObject* O2);
  Intersection() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};

class Exclusion: public Combination {
  Exclusion(BaseObject* O1, BaseObject* O2);
  Exclusion() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};

class Subtraction: public Combination {
  Subtraction(BaseObject* O1, BaseObject* O2);
  Subtraction() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4f& point, const Eigen::Transform<float, 3, Eigen::Projective>& inverse_transform) const override;
};