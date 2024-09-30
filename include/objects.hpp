#pragma once
 
#include <iostream>
#include <vector>
#include <tuple>
#include <array>
#include <Dense>

#include <ray.hpp>
#include <light.hpp>
#include <custom_exceptions.hpp>
#include "defines.h"


/**
 * \class IntersectionPoint objects.hpp
 * 
 * \brief %Intersection point of a Ray and a BaseObject.
 * 
 * The struct carries all relevant information about the point and the object that was hit.
 */
struct IntersectionPoint{
  /** \brief Position of the %intersection point. */
  Eigen::Vector4d point;
  /** \brief Normal vector of the hit object at the point. */
  Eigen::Vector4d normal;

  /** \brief Color of the hit object. */
  ColData color;
  /** \brief Refraction index of the hit object. */
  float index;

  /** \brief Distance of the %intersection point to the start point of the ray. */
  double distance;
  /** \brief True, if the ray comes from within the hit object. */
  bool inside;

  /**
   * \brief Base Constructor for IntersectionPoint.
   */
  IntersectionPoint(Eigen::Vector4d point, Eigen::Vector4d normal, ColData color, float index, double distance, bool inside);

  /**
   * \brief Constructor using 3-dimensional vectors.
   * 
   * The point and normal vectors are converted to homogenous coordinate point vectors (i.e. a 1 is appended).
   */
  IntersectionPoint(Eigen::Vector3d point, Eigen::Vector3d normal, ColData color, float index, double distance, bool inside);
  
  /**
   * \brief Default Constructor for IntersectionPoint.
   * 
   * #point is initialized to the origin, #normal is initialized to the Unit-X-Vector.
   */
  IntersectionPoint();

  /**
   * \brief Comparison operator for two IntersectionPoint's.
   * 
   * \param other second IntersectionPoint to compare *this with
   * 
   * \returns True, if this->#distance is smaller than other.#distance.
   */
  bool operator<(const IntersectionPoint& other) const;
};

/**
 * \brief Multiplication operator for Matrix and IntersectionPoint.
 * 
 * Applies the Matrix on the #point and #direction vectors. 
 * 
 * \param transformation Matrix in homogene coordinate format to be applied on the IntersectionPoint.
 * \param p IntersectionPoint to be transformed.
 */
const IntersectionPoint operator*(const Eigen::Transform<double, 3, Eigen::Projective>& transformation, const IntersectionPoint& p);


/**
 * \class BaseObject objects.hpp
 * 
 * \brief Base class for all objects of a scene.
 */
class BaseObject {
public:
  /** 
   * \brief Dummy Destructor to be implemented by child classes.
   */
  virtual ~BaseObject();

  /**
   * \brief Find all %intersections this object and a Ray.
   * 
   * \param r Ray of which we want to find the %intersections.
   * \param inverse_transform Matrix to transform the Ray into the object space.
   * \param dest Vector in which all found IntersectionPoints are saved.
   * 
   * \returns True, if at least one IntersectionPoint was found.
   */
  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const = 0;
  /**
   * \brief Find all %intersections of the object and a Ray.
   * 
   * Identical to BaseObject::intersect(const Ray&, const Eigen::Transform<double, 3, Eigen::Projective>&, std::vector<IntersectionPoint>&) but assumes that the ray is in object space.
   */
  bool intersect(const Ray& r, std::vector<IntersectionPoint>& dest) const;

  /**
   * \brief Checks if a point is inside this object.
   * 
   * \param point Point to be checked.
   * \param inverse_transform Matrix to transform the point into the object space.
   * 
   * \returns True, if the point lies inside this object.
   */
  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const = 0;
  /**
   * \brief Checks if a point is inside this object.
   * 
   * Converts point to homogenous coordinates.
   */
  bool included(const Eigen::Vector3d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const;
  /**
   * \brief Checks if a point is inside this object.
   * 
   * Assumes, that point is in object space.
   */
  bool included(const Eigen::Vector4d& point);
  /**
   * \brief Checks if a point is inside this object.
   * 
   * Converts point to homogenous coordinates and assumes that it is in object space.
   */
  bool included(const Eigen::Vector3d& point);
};


/**
 * \class RootObject objects.hpp
 * 
 * \brief Root for every object tree describing a scene.
 * 
 * Wraps around the scene by giving convenient functions to interact with the object tree.
 */
class RootObject {
private:
  /**
   * \brief Oject describing the scene.
   * Each BaseObject* should always be attribute of \b only \b one object, to prevent segmentation faults.
   */
  BaseObject* child;
public:
  /**
   * \brief Base Constructor for RootObject.
   */
  RootObject(BaseObject* child);
  RootObject() = delete;

  /**
   * \brief Destructor for RootObject.
   * Frees #child.
   */
  ~RootObject();

  /**
   * \brief Find the neares %intersection point of the scene and a Ray.
   * 
   * Calls intersect on #child and returns the IntersectionPoint with the smallest (non-negative) distance.
   * 
   * \param r Ray of which we want to find the %intersection point.
   * \param dest Pointer to target IntersectionPoint where the found IntersectionPoint should be saved.
   * 
   * \returns True, if an %intersection point was found.
   */
  bool intersect(const Ray& r, IntersectionPoint* dest = nullptr) const;

  /**
   * \brief Check, wether a point is inside some object in the scene.
   * 
   * \param point Point to be checked.
   * 
   * \returns True, if point lies in any object in the scene.
   */
  bool included(const Eigen::Vector4d& point) const;
};


/**
 * \class Primitive objects.hpp
 * 
 * \brief Base class for every primitive object.
 */
class Primitive: public BaseObject {
protected:
  /**
   * \brief Color information
   */
  ColData col;
  /**
   * \brief Refraction index
   */
  float index;

public:
  /**
   * \brief Base Constructor for Primitive.
   */
  Primitive(ColData col, float index);
  /**
   * \brief Default Constructor for Primitive.
   */
  Primitive();

  virtual ~Primitive();
};

/**
 * \class Sphere objects.hpp
 * 
 * \brief A single-color Sphere.
 * 
 * The Sphere is centered at origin, with radius 1.
 */
class Sphere: public Primitive {
public:
  /**
   * \brief Base Constructor for Sphere.
   */
  Sphere(ColData col, float index);
  /**
   * \brief Default Constructor for Sphere.
   */
  Sphere();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;
  
  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

/**
 * \class HalfSpace objects.hpp
 * 
 * \brief A single-color plane.
 * 
 * The plane lies on the origin, perpendicular to #normal.
 * All points lying below the plane, i.e. in opposite direction to #normal,
 * are interpreted as inside the object.
 */
class HalfSpace: public Primitive {
private:
  /**
   * \brief normal vector of the plane
   */
  Eigen::Vector4d normal;
public:
  /**
   * \brief Base Constructor for HalfSpace.
   */
  HalfSpace(ColData col, float index, Eigen::Vector4d normal);
  /**
   * \brief Default Constructor for HalfSpace.
   */
  HalfSpace();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

/**
 * \class Cylinder objects.hpp
 * 
 * \brief An infinitely tall, single-color cylinder.
 * 
 * The cylinders middle axis is the z-axis and it has radius 1.
 */
class Cylinder: public Primitive {
public:
  /**
   * \brief Base Constructor for Cylinder.
   */
  Cylinder(ColData col, float index);
  /**
   * \brief Default Constructor for Cylinder.
   */
  Cylinder();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};


/**
 * \class Transformation objects.hpp
 * 
 * \brief Performs a scalation, rotation or translation on an object.
 * 
 * Given the transformation to perform, it constructs the corresponding matrix
 * as well as its inverse to transform a Ray / point into the transformed object space and
 * back again.
 */
class Transformation: public BaseObject {
private:
  /**
   * \brief BaseObject to be transformed.
   */
  BaseObject* child; 

  /**
   * \brief Forward Transformation Matrix to convert from object space to global space.
   */
  Eigen::Transform<double, 3, Eigen::Projective> transformation;
  /**
   * \brief Inverse Transformation Matrix to convert from global space to object space.
   */
  Eigen::Transform<double, 3, Eigen::Projective> inverse;

public:
  /**
   * \brief Constructs a scaling Transformation.
   * 
   * \param child BaseObject to be transformed.
   * \param ax Scaling factor in x direction.
   * \param ay Scaling factor in y direction.
   * \param az Scaling factor in z direction.
   * 
   * \returns Transformation that applies the described scalation.
   */
  static Transformation* Scaling(BaseObject* child, double ax, double ay, double az);

  /**
   * \brief Constructs a rotation Transformation around the x-axis
   * 
   * \param child BaseObject to be transformed.
   * \param alpha Angle to rotate around.
   * 
   * \returns Transformation that applies the described rotation.
   */
  static Transformation* Rotation_X(BaseObject* child, double alpha);
  /**
   * \brief Constructs a rotation Transformation around the y-axis
   * 
   * \param child BaseObject to be transformed.
   * \param alpha Angle to rotate around.
   * 
   * \returns Transformation that applies the described rotation.
   */
  static Transformation* Rotation_Y(BaseObject* child, double alpha);
  /**
   * \brief Constructs a rotation Transformation around the z-axis
   * 
   * \param child BaseObject to be transformed.
   * \param alpha Angle to rotate around.
   * 
   * \returns Transformation that applies the described rotation.
   */
  static Transformation* Rotation_Z(BaseObject* child, double alpha);

  /**
   * \brief Constructs a translation Transformation.
   * 
   * \param child BaseObject to be transformed.
   * \param ax Translation factor in x direction.
   * \param ay Translation factor in y direction.
   * \param az Translation factor in z direction.
   * 
   * \returns Transformation that applies the described translation.
   */
  static Transformation* Translation(BaseObject* child, double dx, double dy, double dz);

  /** 
   * \brief Helper Constructor for scaling.
   */
  Transformation(BaseObject* child, Eigen::DiagonalMatrix<double, 3> scaling);
  /** 
   * \brief Helper Constructor for rotation.
   */
  Transformation(BaseObject* child, Eigen::AngleAxis<double> rotation);
  /** 
   * \brief Helper Constructor for translation.
   */
  Transformation(BaseObject* child, Eigen::Translation<double, 3> translation);

  virtual ~Transformation();

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;

  /**
   * \brief Getter function for the forward transformation matrix.
   */
  const Eigen::Transform<double, 3, Eigen::Projective>& matrix() const;
  /**
   * \brief Getter function for the inverse transformation matrix.
   */
  const Eigen::Transform<double, 3, Eigen::Projective>& inverse_matrix() const;
};



/**
 * \class Combination objects.hpp
 * 
 * \brief Base class for all Combinations.
 */
class Combination: public BaseObject {
protected:
  /**
   * \brief List of objects that are to be combined.
   */
  std::vector<BaseObject*> objects;

public:
  /**
   * \brief Base Constructor for Combination.
   */
  Combination(std::vector<BaseObject*> objects);
  Combination() = delete;

  virtual ~Combination();
};

/**
 * \class Union objects.hpp
 * 
 * \brief Union of a list of BaseObjects.
 * 
 * Consists of each point, that lies in at least one element of #objects.
 */
class Union: public Combination {
public:
  /** 
   * \brief Base Constructor for Union.
   */
  Union(std::vector<BaseObject*> objects);
  Union() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

/**
 * \class Intersection objects.hpp
 * 
 * \brief Intersection of a list of BaseObjects.
 * 
 * Consists of each point, that lies every element of #objects.
 */
class Intersection: public Combination {
public:
  /** 
   * \brief Base Constructor for Intersection.
   */
  Intersection(std::vector<BaseObject*> objects);
  Intersection() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

/**
 * \class Exclusion objects.hpp
 * 
 * \brief Exclusion of a list of BaseObjects.
 * 
 * Consists of each point, that lies in exactly one element of #objects.
 */
class Exclusion: public Combination {
public:
  /** 
   * \brief Base Constructor for Exclusion.
   */
  Exclusion(std::vector<BaseObject*> objects);
  Exclusion() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};

/**
 * \class Subtraction objects.hpp
 * 
 * \brief Subtraction of a list of BaseObjects.
 * 
 * Consists of each point, that lies in the first element of #objects, and none other.
 */
class Subtraction: public Combination {
public:
  /** 
   * \brief Base Constructor for Subtraction.
   */
  Subtraction(std::vector<BaseObject*> objects);
  Subtraction() = delete;

  virtual bool intersect(const Ray& r, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform, std::vector<IntersectionPoint>& dest) const override;

  virtual bool included(const Eigen::Vector4d& point, const Eigen::Transform<double, 3, Eigen::Projective>& inverse_transform) const override;
};