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
  ColData mirror(black, black, white, white, black, 4);
  ColData mirror_blue(black, black, blue, blue, black, 4);
  ColData mirror_green(black, black, green, green, black, 4);
  ColData mirror_purple(black, black, purple, purple, black, 4);

  // Tibauts Example
  // Sphere* Sphere1 = new Sphere(shiny, 1);


  // Sphere* Sphere2 = new Sphere(shiny, 1);
  // Transformation* scal1 = Transformation::Scaling(Sphere2, 4, 4, 4);

  // HalfSpace* HalfSpace1 = new HalfSpace(shiny, 1, Eigen::Vector4f(1, 0, 0, 0));
  // Transformation* trans1 = Transformation::Translation(HalfSpace1, 0, 0, 2.5);

  // Union* Union1 = new Union(scal1, trans1);


  // Union* Union2 = new Union(Sphere1, Union1);
  // RootObject* R = new RootObject(Union2);

  // LightSource* source1 = new LightSource(Eigen::Vector4f(0, 0, 0, 1), white);
  // LightSource* source2 = new LightSource(Eigen::Vector4f(1, 1, 1, 1), white);

  // Scene scene(128, 4, 2, Eigen::Vector4f(-1, -2, -10, 1), Eigen::Vector4f(0, 0, -20, 1),
  //             LightIntensity(0.1, 0.1, 0.1), 1, 7, {source1, source2}, 
  //             R);


  //Thomas Example
  Sphere* Sphere1 = new Sphere(red_flat, 4);
  Transformation* trans1 = Transformation::Translation(Sphere1, 1, 1, 4);

  HalfSpace* Half1 = new HalfSpace(mirror, 1, Eigen::Vector4f(0, 0, -1, 0));
  Transformation* trans2 = Transformation::Translation(Half1, 0, 0, 7);

  HalfSpace* Half2 = new HalfSpace(mirror_blue, 1, Eigen::Vector4f(1, 0, 0, 0));

  HalfSpace* Half3 = new HalfSpace(mirror_green, 1, Eigen::Vector4f(-1, 0, 0, 0));
  Transformation* trans3 = Transformation::Translation(Half3, 3, 0, 0);

  HalfSpace* Half4 = new HalfSpace(mirror_purple, 1, Eigen::Vector4f(0, -1, 0, 0));
  Transformation* trans4 = Transformation::Translation(Half4, 0, 3, 0);

  Union* union1 = new Union(trans1, trans2);
  Union* union2 = new Union(union1, Half2);
  Union* union3 = new Union(union2, trans3);
  Union* union4 = new Union(union3, trans4);

  RootObject* root = new RootObject(union4);

  LightSource* source1 = new LightSource(Eigen::Vector4f(1.5, 1, -2, 1), white);

  Scene scene(256, 3, 2, Eigen::Vector4f(0, 0, 0, 1), Eigen::Vector4f(1.5, 1, -2, 1),
              LightIntensity(0.1, 0.1, 0.1), 1, 10, {source1}, root);

  std::cout << "starting:\n";

  cv::Mat_<cv::Vec3b> img = scene.generate();

  std::cout << img;

  cv::imwrite("output.png", img);

  return 0;
}