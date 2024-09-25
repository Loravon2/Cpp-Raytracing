#include <iostream>
#include <Dense>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "math.h"

#include <scene.hpp>
#include <ray.hpp>
#include <objects.hpp>
#include <light.hpp>
#include "defines.h"

int main() {
  std::ifstream inp;
  inp.open("../tibauts_example.json", std::ios::in);

  if (not inp.is_open()) {
    std::cout << "cry";
    exit(1);
  }

  Scene scene = Scene::read_parameters(inp);

  std::cout << "starting:\n";

  cv::Mat_<cv::Vec3b> img = scene.generate();

  std::cout << img;

  cv::imwrite("output.png", img);

  return 0;
}