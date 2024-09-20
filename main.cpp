#include <iostream>
#include <Dense>
#include <opencv2/opencv.hpp>
#include "math.h"

#include <scene.hpp>
#include <ray.hpp>
#include <objects.hpp>
#include <light.hpp>
#include "defines.h"

int main() {
  LightIntensity white(1, 1, 1);
  LightIntensity red(1, 0, 0);
  LightIntensity blue(0, 0, 1);
  LightIntensity green(0, 1, 0);
  LightIntensity black(0, 0, 0);
  LightIntensity purple(102.0 / 255.0, 51.0 / 255.0, 153.0 / 255.0);

  ColData shiny(white, white, white, white, white, 1);
  ColData red_flat(red, red, red, red, black, 2);
  ColData reflective_black(black, black, white * 0.2, white * 0.2, black, 3);
  ColData mirror(black, black, white, white, black, 4);
  ColData mirror_blue(black, black, blue, blue, black, 4);
  ColData mirror_green(green, green, green, green, black, 4);
  ColData mirror_purple(black, black, purple, purple, black, 4);

  ColData my_back_wall(white, white * 0.5, white, white, blue, 2);

  // Tibauts Example
  // Sphere* Sphere1 = new Sphere(shiny, 1);


  // Sphere* Sphere2 = new Sphere(shiny, 1);
  // Transformation* scal1 = Transformation::Scaling(Sphere2, 4, 4, 4);

  // HalfSpace* HalfSpace1 = new HalfSpace(shiny, 1, Eigen::Vector4f(1, 0, 0, 0));
  // Transformation* trans1 = Transformation::Translation(HalfSpace1, 0, 0, 2.5);

  // Union* Union1 = new Union({Sphere1, scal1, trans1});

  // RootObject* R = new RootObject(Union1);

  // LightSource* source1 = new LightSource(Eigen::Vector4f(0, 0, 0, 1), white);
  // LightSource* source2 = new LightSource(Eigen::Vector4f(1, 1, 1, 1), white);

  // Scene scene(128, 4, 2, Eigen::Vector4f(-1, -2, -10, 1), Eigen::Vector4f(0, 0, -20, 1),
  //             LightIntensity(0.1, 0.1, 0.1), 1, 7, {source1, source2}, 
  //             R);


  //Thomas Example
  // Sphere* Sphere1 = new Sphere(red_flat, 4);
  // Transformation* trans1 = Transformation::Translation(Sphere1, 1, 1, 4);

  // HalfSpace* Half1 = new HalfSpace(mirror, 1, Eigen::Vector4f(0, 0, -1, 0));
  // Transformation* trans2 = Transformation::Translation(Half1, 0, 0, 7);

  // HalfSpace* Half2 = new HalfSpace(mirror_blue, 1, Eigen::Vector4f(1, 0, 0, 0));

  // HalfSpace* Half3 = new HalfSpace(mirror_green, 1, Eigen::Vector4f(-1, 0, 0, 0));
  // Transformation* trans3 = Transformation::Translation(Half3, 3, 0, 0);

  // HalfSpace* Half4 = new HalfSpace(mirror_purple, 1, Eigen::Vector4f(0, -1, 0, 0));
  // Transformation* trans4 = Transformation::Translation(Half4, 0, 3, 0);

  // HalfSpace* Half5 = new HalfSpace(mirror, 1, Eigen::Vector4f(0, 0, 1, 0));
  // Transformation* trans5 = Transformation::Translation(Half5, 0, 0, -3);

  // HalfSpace* Half6 = new HalfSpace(mirror, 1, Eigen::Vector4f(0, 1, 0, 0));

  // Union* union1 = new Union({trans1, trans2, Half2, trans3, trans4, trans5, Half6});

  // RootObject* root = new RootObject(union1);

  // LightSource* source1 = new LightSource(Eigen::Vector4f(1.5, 1, -2, 1), white);

  // Scene scene(64, 3, 2, Eigen::Vector4f(0, 0, 0, 1), Eigen::Vector4f(1.5, 1, -2, 1),
  //             LightIntensity(0.1, 0.1, 0.1), 1, 5, {source1}, root);


  //Intersecting Spheres
  // Sphere* Sphere1 = new Sphere(red_flat, 1);
  // Sphere* Sphere2 = new Sphere(mirror_green, 1);

  // Transformation* Trans1 = Transformation::Translation(Sphere2, 1.0 / sqrtf(3), 1.0 / sqrtf(3), -1.0 / sqrtf(3));
  // Transformation* Scal1 = Transformation::Scaling(Trans1, 2, 2, 2);

  // Subtraction* Sub1 = new Subtraction({Sphere1, Scal1});

  // HalfSpace* HalfSpace1 = new HalfSpace(reflective_black, 1, Eigen::Vector4f(-1, 0, 0, 0));
  // Transformation* Rot1 = Transformation::Rotation_Y(HalfSpace1, -M_PI_4);
  // Transformation* Trans2 = Transformation::Translation(Rot1, 3, 0, 0);
  // Transformation* Trans3 = Transformation::Translation(Trans2, 0, 0, 4);

  // Union* Union1 = new Union({Sub1, Trans3});

  // RootObject* root = new RootObject(Union1);

  // LightSource* source1 = new LightSource(Eigen::Vector4f(0, 4, 0, 1), white);

  // Scene scene(64, 3, 2, Eigen::Vector4f(-1.5, -1, -2, 1), Eigen::Vector4f(0, 0, -4, 1), 
  //             black, 1, 7, {source1}, root);

  //Exclusion of Spheres
  // Sphere* Sphere1 = new Sphere(red_flat, 1);
  // Sphere* Sphere2 = new Sphere(mirror_green, 1);

  // Transformation* Trans1 = Transformation::Translation(Sphere2, 1.0 / sqrtf(3), 1.0 / sqrtf(3), -1.0 / sqrtf(3));
  // Transformation* Scal1 = Transformation::Scaling(Trans1, 2, 2, 2);

  // Subtraction* Sub1 = new Subtraction({Sphere1, Scal1});

  // HalfSpace* HalfSpace1 = new HalfSpace(reflective_black, 1, Eigen::Vector4f(-1, 0, 0, 0));
  // Transformation* Rot1 = Transformation::Rotation_Y(HalfSpace1, -M_PI_4);
  // Transformation* Trans2 = Transformation::Translation(Rot1, 3, 0, 0);
  // Transformation* Trans3 = Transformation::Translation(Trans2, 0, 0, 4);

  // Sphere* Sphere3 = new Sphere(mirror_purple, 1);
  // Subtraction* Ex1 = new Subtraction({Sphere3, Sub1});

  // Union* Union1 = new Union({Ex1, Trans3});

  // RootObject* root = new RootObject(Union1);

  // LightSource* source1 = new LightSource(Eigen::Vector4f(0, 4, 0, 1), white);
  // LightSource* source2 = new LightSource(Eigen::Vector4f(0, 0, -4, 1), white);

  // Scene scene(64, 3, 2, Eigen::Vector4f(-1.5, -1, -2, 1), Eigen::Vector4f(0, 0, -4, 1), 
  //             black, 1, 7, {source1, source2}, root);

  // Square?
  HalfSpace* right = new HalfSpace(red_flat, 1, Eigen::Vector4f::UnitX());
  Transformation* T1 = Transformation::Translation(right, 0.5, 0, 0);

  HalfSpace* left = new HalfSpace(red_flat, 1, -Eigen::Vector4f::UnitX());
  Transformation* T2 = Transformation::Translation(left, -0.5, 0, 0);

  HalfSpace* top = new HalfSpace(red_flat, 1, Eigen::Vector4f::UnitY());
  Transformation* T3 = Transformation::Translation(top, 0, 0.5, 0);

  HalfSpace* bottom = new HalfSpace(red_flat, 1, -Eigen::Vector4f::UnitY());
  Transformation* T4 = Transformation::Translation(bottom, 0, -0.5, 0);

  HalfSpace* front = new HalfSpace(red_flat, 1, Eigen::Vector4f::UnitZ());
  Transformation* T5 = Transformation::Translation(front, 0, 0, 0.5);

  HalfSpace* back = new HalfSpace(red_flat, 1, -Eigen::Vector4f::UnitZ());
  Transformation* T6 = Transformation::Translation(back, 0, 0, -0.5);

  Intersection* cube = new Intersection({T1, T2, T3, T4, T5, T6});

  Transformation* rot = Transformation::Rotation_Y(cube, M_PI / 5);

  RootObject* root = new RootObject(rot);

  LightSource* source1 = new LightSource(Eigen::Vector4f(0, 0, -4, 1), white);

  Scene scene(64, 3, 2, Eigen::Vector4f(-1.5, -1, -2, 1), Eigen::Vector4f(0, 0, -4, 1), 
              black, 1, 7, {source1}, root);

  std::cout << "starting:\n";

  cv::Mat_<cv::Vec3b> img = scene.generate();

  std::cout << img;

  cv::imwrite("output.png", img);

  return 0;
}