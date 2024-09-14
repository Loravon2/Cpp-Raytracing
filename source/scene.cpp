#include <scene.hpp>

Scene::Scene(float dpi, float width, float height,
            Eigen::Vector4f position, Eigen::Vector4f observer,
            LightIntensity ambient_light, float index,
            unsigned max_recursion_depth,
            std::vector<LightSource*> sources, RootObject* objects):  
          dpi(dpi), width(width), height(height), position(position), observer(observer), 
          ambient_light(ambient_light), index(index), max_recursion_depth(max_recursion_depth),
          sources(sources), objects(objects)
          {}

Scene::~Scene() {
  std::cout << "Destructing Scene at " << this << std::endl;

  for (LightSource* source : sources) {
    delete source;
  }

  delete objects;
}



LightIntensity Scene::trace_ray(const Ray& ray, unsigned depth) const {
  IntersectionPoint ip;

  if (!objects->intersect(ray, &ip)) {
    return LightIntensity();
  }

  ColData texture = ip.color;

  LightIntensity value = texture.ambient * ambient_light;
  if(value.at(0) < -EPSILON) {
    std::cout << "Negative value after ambient: " << value << std::endl;
    exit(1);
  }
  
  for (LightSource* ls : sources) {
    Eigen::Vector4f light_dir = ls->pos() - ip.point;

    if (light_dir == Eigen::Vector4f::Zero()) {
      continue;
    }

    Ray light_connection(ip.point, light_dir, ray.index());

    IntersectionPoint light_ip;
    if (objects->intersect(light_connection, &light_ip) && light_ip.distance < light_dir.norm()) {
      continue;
    }

    Ray light_reflection = (-light_connection).reflect(ip.point, ip.normal);

    value += texture.diffuse * ls->rgb() * abs(light_connection.direction().dot(ip.normal));// * (1 / light_dir.norm()) * (1 / light_dir.norm()); //might wanna think about that if we want to add inverse square law (maybe scaled by some coefficient?)
    if(value.at(0) < -EPSILON) {
      std::cout << "Negative value after diffuse: " << value << std::endl;
      exit(1);
    }

    if (light_reflection.direction().dot((-ray).direction()) > 0) {
      value += texture.specular * ls->rgb() * powf32(light_reflection.direction().dot((-ray).direction()), texture.shininess);// * (1 / light_dir.norm()) * (1 / light_dir.norm());
    }
    if(value.at(0) < -EPSILON) {
      std::cout << "Negative value after specular: " << value << std::endl;
      exit(1);
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

  if (ray.index() < ip.index || acosf32(ray.direction().dot(ip.normal)) < asinf32(ray.index() / ip.index)) { // otherwise we have total reflection
    Ray refraction = ray.refract(ip.point, ip.normal, ip.index);
    value += texture.refracted * trace_ray(refraction, depth+1);
    if(value.at(0) < -EPSILON) {
      std::cout << "Negative value after refrected: " << value << std::endl;
      exit(1);
    }
  }

  return value;
}


cv::Mat_<cv::Vec3b> Scene::generate() {
  cv::Mat_<cv::Vec3b> pixel_data(dpi * height, dpi * width);

  for (unsigned i = 0; i < dpi * height; i++) {
    for (unsigned j = 0; j < dpi * width; j++) {
      Eigen::Vector4f Pij = position + 1.0 / dpi * (j * Eigen::Vector4f::UnitX() + i * Eigen::Vector4f::UnitY()) + 1 / (2*dpi) * (Eigen::Vector4f::UnitX() + Eigen::Vector4f::UnitY());
      Ray ray(observer, Pij - observer, index);

      LightIntensity val = trace_ray(ray, 0);
      std::cout << val;

      for (unsigned k = 0; k < NUM_COL; k++) {
        pixel_data(dpi * height - i - 1, j)[k] = 255 * val.at(NUM_COL - k - 1);
      }
    }
  }

  return pixel_data;
}