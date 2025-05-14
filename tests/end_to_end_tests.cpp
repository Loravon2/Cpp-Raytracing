#include <Dense>
#include <cassert>
#include <string>
#include <sstream>

#include <composite.hpp>
#include <light.hpp>
#include <objects.hpp>
#include <ray.hpp>
#include <scene.hpp>
#include <custom_exceptions.hpp>
#include <defines.h>

std::string scene_str = ""
"{\"screen\": {\"dpi\": 128, \"dimensions\": [2, 4], \"position\": [-1, -2, -10], \"observer\": [0, 0, -20]}, "
"\"medium\": {\"ambient\": \"white\", \"index\": 1, \"recursion\": 0}, "
"\"sources\": [{\"position\": [0, 0, -20],\"intensity\": [1, 1, 1]}], "
"\"objects\": [{\"cylinder\": {\"position\": [0, 0, 0], \"radius\": 2, \"axis\": [1, 1, 0], \"color\": {"
"\"ambient\": \"white\", \"diffuse\": [1, 1, 1], \"specular\": [1, 1, 1], \"reflected\": [1, 1, 1], \"refracted\": \"white\", \"shininess\": 1}, "
"\"index\": 1}}]}";

int main() {
  std::cout << scene_str << std::endl;

  std::istringstream scene_buf(scene_str);
  Scene scene = Scene::read_parameters(scene_buf);

  Ray r1(Eigen::Vector4d(0, 0, -20, 1), Eigen::Vector4d(0, 0, 1, 0), 1);
  LightIntensity li = scene.trace_ray(r1, 0);
  
  CUSTOM_ASSERT(li.at(0) == 1 and li.at(1) == 1 and li.at(2) == 1);

  return 0;
}