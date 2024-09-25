#include <scene.hpp>

using json = nlohmann::json;

const std::map<std::string, Scene::action_t> Scene::action_handler = {
  {"sphere", &Scene::read_sphere},              {"halfSpace", &Scene::read_half_space},
  {"scaling", &Scene::read_scaling},            {"rotation", &Scene::read_rotation},
  {"translation", &Scene::read_translation},    {"union", &Scene::read_union},
  {"intersection", &Scene::read_intersection},  {"exclusion", &Scene::read_exclusion},
  {"subtraction", &Scene::read_subtraction}
};

const std::array<Scene::rotation_t, 3> Scene::rotation_handler = {
  &Transformation::Rotation_X,
  &Transformation::Rotation_Y,
  &Transformation::Rotation_Z
};


LightSource* Scene::read_source(nlohmann::json& descr) {
  std::array<float, 3> raw_pos = descr.at("position");
  std::array<float, 3> intensity = descr.at("intensity");

  Eigen::Vector3f pos(raw_pos[0], raw_pos[1], raw_pos[2]);

  LightSource* source = new LightSource(pos, LightIntensity(intensity));

  return source;
}

ColData Scene::read_col_data(nlohmann::json& descr) {
  LightIntensity amb(descr.at("ambient"));
  LightIntensity diff(descr.at("diffuse"));
  LightIntensity spec(descr.at("specular"));
  LightIntensity refl(descr.at("reflected"));
  LightIntensity refr(descr.at("refracted"));
  float shiny = descr.at("shininess");

  return ColData(amb, diff, spec, refl, refr, shiny);
}

std::vector<BaseObject*> Scene::read_obj_list(nlohmann::json& descr) {
  std::vector<BaseObject*> objects;

  for (auto [_, j] : descr.items()) {
    BaseObject* obj = action_handler.at(j.begin().key())(j.begin().value());
    objects.push_back(obj);
  }

  return objects;
}


BaseObject* Scene::read_sphere(nlohmann::json& descr) {
  ColData col = read_col_data(descr.at("color"));
  float ind = descr.at("index");

  BaseObject* obj = new Sphere(col, ind);

  float rad = descr.at("radius");
  if (rad != 1.0) {
    obj = Transformation::Scaling(obj, rad, rad, rad);
  }

  std::array<float, 3> pos = descr.at("position");
  if (pos[0] != 0 or pos[1] != 0 or pos[2] != 0) {
    obj = Transformation::Translation(obj, pos[0], pos[1], pos[2]);
  }

  return obj;
}

BaseObject* Scene::read_half_space(nlohmann::json& descr) {
  ColData col = read_col_data(descr.at("color"));
  float ind = descr.at("index");

  std::array<float, 3> normal_arr = descr.at("normal");
  Eigen::Vector4f normal(normal_arr[0], normal_arr[1], normal_arr[2], 0);

  BaseObject* obj = new HalfSpace(col, ind, normal);

  std::array<float, 3> pos = descr.at("position");
  if (pos[0] != 0 or pos[1] != 0 or pos[2] != 0) {
    obj = Transformation::Translation(obj, pos[0], pos[1], pos[2]);
  }

  return obj;
}

BaseObject* Scene::read_scaling(nlohmann::json& descr) {
  std::array<float, 3> fac = descr.at("factors");

  auto j = descr["subject"].begin();
  BaseObject* subj = action_handler.at(j.key())(j.value());

  BaseObject* obj = Transformation::Scaling(subj, fac[0], fac[1], fac[2]);

  return obj;
}

BaseObject* Scene::read_rotation(nlohmann::json& descr) {
  float ang = descr["angle"].get<float>() / 180.0 * M_PI;
  unsigned dir = descr["direction"];

  auto j = descr["subject"].begin();
  BaseObject* subj = action_handler.at(j.key())(j.value());

  BaseObject* obj = rotation_handler.at(dir)(subj, ang);

  return obj;
}

BaseObject* Scene::read_translation(nlohmann::json& descr) {
  std::array<float, 3> fac = descr.at("factors");

  auto j = descr["subject"].begin();
  BaseObject* subj = action_handler.at(j.key())(j.value());

  BaseObject* obj = Transformation::Translation(subj, fac[0], fac[1], fac[2]);

  return obj;
}

BaseObject* Scene::read_union(nlohmann::json& descr) {
  std::vector<BaseObject*> subj = read_obj_list(descr);

  BaseObject* obj = new Union(subj);

  return obj;
}

BaseObject* Scene::read_intersection(nlohmann::json& descr) {
  std::vector<BaseObject*> subj = read_obj_list(descr);

  BaseObject* obj = new Intersection(subj);

  return obj;
}

BaseObject* Scene::read_exclusion(nlohmann::json& descr) {
  std::vector<BaseObject*> subj = read_obj_list(descr);

  BaseObject* obj = new Exclusion(subj);

  return obj;
}

BaseObject* Scene::read_subtraction(nlohmann::json& descr) {
  std::vector<BaseObject*> subj = read_obj_list(descr);

  BaseObject* obj = new Subtraction(subj);

  return obj;
}


Scene Scene::read_parameters(std::istream& input) {
  json data = json::parse(input);

  json screen_info = data.at("screen");
  float dpi = screen_info.at("dpi");
  std::array<float, 2> dim = screen_info.at("dimensions");
  std::array<float, 3> pos = screen_info.at("position");
  std::array<float, 3> obs = screen_info.at("observer");

  json medium_info = data.at("medium");
  std::array<float, 3> amb = medium_info.at("ambient");
  float index = medium_info.at("index");
  unsigned recursion = medium_info.at("recursion");

  std::vector<LightSource*> sources;
  for (auto [_, val] : data.at("sources").items()) {
    LightSource* src = read_source(val);
    sources.push_back(src);
  }

  BaseObject* objects = read_union(data.at("objects"));
  RootObject* root = new RootObject(objects);

  return Scene(dpi, dim[0], dim[1], 
               Eigen::Vector4f(pos[0], pos[1], pos[2], 1), Eigen::Vector4f(obs[0], obs[1], obs[2], 1),
               LightIntensity(amb), index, recursion, sources, root);
}

