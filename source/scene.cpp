#include <scene.hpp>

Scene::Scene(float dpi, float L_x, float L_y,
            Eigen::Vector4d position, Eigen::Vector4d observer,
            LightIntensity ambient_light, float global_index,
            unsigned max_recursion_depth,
            std::vector<LightSource*> sources, RootObject* objects):  
          dpi(dpi), L_x(L_x), L_y(L_y), position(position), observer(observer), 
          ambient_light(ambient_light), global_index(global_index), object_indexs(), 
          max_recursion_depth(max_recursion_depth), sources(sources), objects(objects)
          {}

Scene::~Scene() {
  #ifdef DEBUG
    std::cout << "\nDestructing Scene at " << this << std::endl;
  #endif

  for (LightSource* source : sources) {
    delete source;
  }

  delete objects;
}



LightIntensity Scene::trace_ray(const Ray& ray, unsigned depth) {
  IntersectionPoint ip;

  if (!objects->intersect(ray, &ip)) {
    return LightIntensity();
  }

  ColData texture = ip.color;

  float index = global_index;
  if (ip.inside) {
    try {
      object_indexs.at(ip.index)--;

      if (object_indexs.at(ip.index) == 0) {
        object_indexs.erase(ip.index);
      }

      if (object_indexs.empty()) {
        index = global_index;
      }
      else {
        index = object_indexs.begin()->first; // some key that still remains in object_indexs, i.e. an object that we havent left yet
      }
    }
    catch (std::out_of_range&) {
      index = global_index;
    }
  }
  else {
    index = ip.index;

    try {
      object_indexs.at(ip.index)++;
    }
    catch (std::out_of_range&) {
      object_indexs[ip.index] = 1;
    }
  }

  LightIntensity value = texture.ambient * ambient_light;
  
  for (LightSource* ls : sources) {
    Eigen::Vector4d light_dir = ls->pos() - ip.point;

    if (light_dir == Eigen::Vector4d::Zero()) {
      continue;
    }

    Ray light_connection;
    // to combat shadow acne
    Ray light_connection1(ip.point + EPSILON * ip.normal, light_dir, ray.index());
    Ray light_connection2(ip.point - EPSILON * ip.normal, light_dir, ray.index()); // if light source is inside object

    IntersectionPoint light_ip1; IntersectionPoint light_ip2;
    if (objects->intersect(light_connection1, &light_ip1) && light_ip1.distance < light_dir.norm()) {
      // light_ip1 is discarded
      if (objects->intersect(light_connection2, &light_ip2) && light_ip2.distance < light_dir.norm()) {
        // both are not valid
        continue;
      }

      light_connection = light_connection2;
    }
    else {
      light_connection = light_connection1;
    }

    Ray light_reflection = (-light_connection).reflect(ip.point, ip.normal);

    value += texture.diffuse * ls->rgb() * abs(light_connection.direction().dot(ip.normal));

    if (light_reflection.direction().dot((-ray).direction()) > 0) {
      value += texture.specular * ls->rgb() * (double) powf64(light_reflection.direction().dot((-ray).direction()), texture.shininess);
    }
  }

  if (depth == max_recursion_depth) {
    return value;
  }

  Ray reflection = ray.reflect(ip.point, ip.normal);
  value += texture.reflected * trace_ray(reflection, depth+1);
  if(value.at(0) < -EPSILON) {
    std::cout << "Negative value after reflected: " << value << std::endl;
    exit(1);
  }

  if (ray.index() < index || acosf64(ray.direction().dot(ip.normal)) < asinf64(ray.index() / index)) { // otherwise we have total reflection
    Ray refraction = ray.refract(ip.point, ip.normal, index);
    value += texture.refracted * trace_ray(refraction, depth+1);
    if(value.at(0) < -EPSILON) {
      std::cout << "Negative value after refrected: " << value << std::endl;
      exit(1);
    }
  }

  return value;
}

// credit to leemes on stackoverflow for the implementation (https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf)
void Scene::progress_bar(float progress) {
  std::cout << "[";

  int pos = BAR_WIDTH * progress;
  for (int i=0; i < BAR_WIDTH; i++) {
    if (i < pos) {
      std::cout << "=";
    }
    else if (i == pos) {
      std::cout << ">";
    }
    else {
      std::cout << " ";
    }
  }

  std::cout << "] " << int(progress * 100.0) << "%\r" << std::flush;
}

cv::Mat_<cv::Vec3b> Scene::generate() {
  cv::Mat_<cv::Vec3b> pixel_data(dpi * L_x, dpi * L_y);

  for (unsigned i = 0; i < dpi * L_x; i++) {
    for (unsigned j = 0; j < dpi * L_y; j++) {
      Eigen::Vector4d Pij = position + 1.0 / dpi * (i * Eigen::Vector4d::UnitX() + j * Eigen::Vector4d::UnitY()) + 1 / (2*dpi) * (Eigen::Vector4d::UnitX() + Eigen::Vector4d::UnitY());
      Ray ray(observer, Pij - observer, global_index);

      LightIntensity val = trace_ray(ray, 0);

      for (unsigned k = 0; k < NUM_COL; k++) {
        pixel_data(dpi * L_x - i - 1, j)[k] = 255 * val.at(NUM_COL - k - 1);
      }

      float progress = (float) i / (dpi * L_x) + (float) j / (dpi * dpi * L_x * L_y);
      progress_bar(progress);
    }
  }

  std::cout << std::endl;

  return pixel_data;
}