#pragma once

#include <string>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include "math.h"
#include <Dense>
#include <opencv2/opencv.hpp>
#include <json.hpp>

#include <objects.hpp>
#include <light.hpp>
#include <ray.hpp>
#include "defines.h"

class Scene {
private:
  typedef BaseObject* (*action_t)(nlohmann::json&);
  typedef Transformation* (*rotation_t)(BaseObject*, float);
  static const std::map<std::string, action_t> action_handler;
  static const std::array<rotation_t, 3> rotation_handler;

  static LightSource* read_source(nlohmann::json& descr);

  static ColData read_col_data(nlohmann::json& descr);
  static std::vector<BaseObject*> read_obj_list(nlohmann::json& descr);

  static BaseObject* read_sphere(nlohmann::json& descr);
  static BaseObject* read_half_space(nlohmann::json& descr);
  static BaseObject* read_scaling(nlohmann::json& descr);
  static BaseObject* read_rotation(nlohmann::json& descr);
  static BaseObject* read_translation(nlohmann::json& descr);
  static BaseObject* read_union(nlohmann::json& descr);
  static BaseObject* read_intersection(nlohmann::json& descr);
  static BaseObject* read_exclusion(nlohmann::json& descr);
  static BaseObject* read_subtraction(nlohmann::json& descr);

  float dpi, width, height;

  Eigen::Vector4f position, observer;

  LightIntensity ambient_light;
  float global_index;
  std::map<float, unsigned> object_indexs;

  unsigned max_recursion_depth;

  std::vector<LightSource*> sources;
  RootObject* objects;

  LightIntensity trace_ray(const Ray& ray, unsigned depth);
  
public:
  static Scene read_parameters(std::istream& input);
  
  Scene(float dpi, float width, float height,
        Eigen::Vector4f position, Eigen::Vector4f observer,
        LightIntensity ambient_light, float global_index,
        unsigned max_recursion_depth,
        std::vector<LightSource*> sources, RootObject* objects);
  Scene();

  ~Scene();

  cv::Mat_<cv::Vec3b> generate();
};