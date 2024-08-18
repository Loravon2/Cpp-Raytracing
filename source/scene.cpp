#include <scene.hpp>

LightIntensity Scene::trace_ray(const Ray& ray, unsigned depth) const {
  IntersectionPoint ip;

  if (!objects->intersect(ray, &ip)) {
    return LightIntensity();
  }

  ColData texture = ip.color;

  LightIntensity value = texture.ambient * ambient_light;
  
  for (LightSource* ls : sources) {
    Ray light_connection(ip.point, ls->pos() - ip.point, ray.index());

    IntersectionPoint light_ip;
    if (objects->intersect(light_connection, &light_ip) && light_ip.distance < (ls->pos() - ip.point).norm()) {
      continue;
    }

    Ray light_reflection = (-light_connection).reflect(ip.point, ip.normal);

    value += texture.diffuse * ls->rgb() * (light_connection.direction().dot(ip.normal));
    value += texture.specular * ls->rgb() * (light_reflection.direction().dot(ray.direction()));
  }

  if (depth == max_recursion_depth) {
    return value;
  }

  Ray reflection = ray.reflect(ip.point, ip.normal);
  Ray refraction = ray.refract(ip.point, ip.normal, ip.index);

  value += texture.reflected * trace_ray(reflection, depth+1);
  value += texture.refracted * trace_ray(refraction, depth+1);

  return value;
}