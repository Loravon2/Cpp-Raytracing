#include <scene.hpp>

static Scene::Scene read_parameters(std::istream& input){

        // open json file:
        std::ifstream scene_file(input, std::ifstream::binary);
        if (!scene_file.is_open()){
            cout << "CAN'T OPEN THE GIVEN FILE!" // or with try/catch 
            return 1;
        }
        Json::Value scene_data;
        scene_file >> scene_data ;

        // read parameters from scene_data
        // read from screen:
        float dpi = scene_data["screen"]["dpi"].asFloat() ;
        float height = scene_data["screen"]["dimensions"][0].asFloat();
        float width = scene_data["screen"]["dimensions"][1].asFloat() ;

        Eigen::Vector4f position = Eigen::Vector4f (scene_data["screen"]["position"][0].asFloat(),
                                                    scene_data["screen"]["position"][1].asFloat(),
                                                    scene_data["screen"]["position"][2].asFloat(),
                                                    scene_data["screen"]["position"][3].asFloat()) ;

        Eigen::Vector4f observer = Eigen::Vector4f( scene_data["screen"]["observer"][0].asFloat(),
                                                    scene_data["screen"]["observer"][1].asFloat(),
                                                    scene_data["screen"]["observer"][2].asFloat(),
                                                    scene_data["screen"]["observer"][3].asFloat()) ;

        float index = scene_data["medium"]["index"].asFloat;
        unsigned max_recursion_depth = scene_data["medium"]["recursion"].asUINT();

        LightIntensity ambient_light = LightIntensity(scene_data["medium"]["ambient"][0].asFloat(),
                                                      scene_data["medium"]["ambient"][1].asFloat(),
                                                      scene_data["medium"]["ambient"][2].asFloat());

        // add a loop to itearte over the light sources and store it:
        //Json::Value light_sources_pos = scene_data["sources"]["position"];
        //Json::Value light_sources_intensity = scene_data["sources"]["position"];

        Json::Value light_sources = scene_data["sources"];
        std::vector<LightSource*> sources = {nullptr};
        
        Eigen::Vector3f light_source_intensity;
    
        for(unsigned i=0; i<sources.size();i++){
            const Json::Value& source = sources[i];
            Eigen::Vector4f light_source_position = Eigen::Vector4f(source["position"][0].asFloat(),
                                                                    source["position"][1].asFloat(),
                                                                    source["position"][2].asFloat(),
                                                                    source["position"][3].asFloat());

            
            LightIntensity light_source_intensity = LightIntensity(Eigen::Vector4f(source["intensity"][0].asFloat(),
                                                    source["intensity"][1].asFloat(),
                                                    source["intensity"][2].asFloat(),
                                                    source["intensity"][3].asFloat());

            


            sources.pushback(new LightIntensity(light_source_position,light_source_intensity))
        }

      // read objects (to come..):
}

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
  
  for (LightSource* ls : sources) {
    Eigen::Vector4f light_dir = ls->pos() - ip.point;

    if (light_dir == Eigen::Vector4f::Zero()) {
      continue;
    }

    if (light_dir.dot(ip.normal) < 0) {
      continue; // the light source lies in the shadow of the object we just hit
    }

    Ray light_connection(ip.point, light_dir, ray.index());

    IntersectionPoint light_ip;
    if (objects->intersect(light_connection, &light_ip) && light_ip.distance < light_dir.norm()) {
      continue;
    }

    Ray light_reflection = (-light_connection).reflect(ip.point, ip.normal);

    value += texture.diffuse * ls->rgb() * (light_connection.direction().dot(ip.normal));// * (1 / light_dir.norm()) * (1 / light_dir.norm()); //might wanna think about that if we want to add inverse square law (maybe scaled by some coefficient?)
    value += texture.specular * ls->rgb() * powf32(light_reflection.direction().dot((-ray).direction()), texture.shininess);// * (1 / light_dir.norm()) * (1 / light_dir.norm());
  }

  if (depth == max_recursion_depth) {
    return value;
  }

  Ray reflection = ray.reflect(ip.point, ip.normal);
  value += texture.reflected * trace_ray(reflection, depth+1);

  if (ray.index() < ip.index || acosf32(ray.direction().dot(ip.normal)) < asinf32(ray.index() / ip.index)) { // otherwise we have total reflection
    Ray refraction = ray.refract(ip.point, ip.normal, ip.index);
    value += texture.refracted * trace_ray(refraction, depth+1);
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