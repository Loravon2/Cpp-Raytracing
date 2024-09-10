#pragma once

#include <vector>
#include <iostream>
#include "math.h"
#include <Eigen>
#include <opencv2/opencv.hpp>

#include <jsoncpp/json/json.h>
#include <json/value.h>
#include <fstream>

#include <objects.hpp>
#include <light.hpp>
#include <ray.hpp>
#include "defines.h"

class Scene {
private:
  float dpi, width, height;

  Eigen::Vector4f position, observer;

  LightIntensity ambient_light;
  float index;

  unsigned max_recursion_depth;

  std::vector<LightSource*> sources;
  RootObject* objects;

  LightIntensity trace_ray(const Ray& ray, unsigned depth) const;
  
public:
  static Scene read_parameters(std::istream& input); // to be implemented with "json file input" / main way to create Scene object
  
  Scene(float dpi, float width, float height,
        Eigen::Vector4f position, Eigen::Vector4f observer,
        LightIntensity ambient_light, float index,
        unsigned max_recursion_depth,
        std::vector<LightSource*> sources, RootObject* objects);
  Scene();

  ~Scene();

  cv::Mat_<cv::Vec3b> generate();
};