#include <scene.hpp>

Scene Scene::read_parameters(std::istream& input){

        // open json file:
        /*std::ifstream scene_file(input, std::ifstream::binary);
        if (!scene_file.is_open()){
            std::cout << "CAN'T OPEN THE GIVEN FILE!"; // or with try/catch 
          
        }
        Json::Value scene_data;
        scene_file >> scene_data ;*/


        // check if input is valid:
        if (!input) {
        std::cerr << "Invalid input stream!" << std::endl;
        return Scene();
        }

        // Read JSON data from the input stream
        Json::Value scene_data;
        input >> scene_data ;   
        

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

        float index = scene_data["medium"]["index"].asFloat();
        unsigned max_recursion_depth = scene_data["medium"]["recursion"].asUInt();

        LightIntensity ambient_light = LightIntensity(scene_data["medium"]["ambient"][0].asFloat(),
                                                      scene_data["medium"]["ambient"][1].asFloat(),
                                                      scene_data["medium"]["ambient"][2].asFloat());


        Json::Value light_sources = scene_data["sources"];
        std::vector<LightSource*> sources ;  // could use uniquepointers to deallocate memory when we are finished with it and for better performance 
        
        Eigen::Vector3f light_source_intensity;
    
        for(unsigned i=0; i<sources.size();i++){
            const Json::Value& source = sources[i];
            Eigen::Vector4f light_source_position = Eigen::Vector4f(source["position"][0].asFloat(),
                                                                    source["position"][1].asFloat(),
                                                                    source["position"][2].asFloat());
                                                                    

            
            LightIntensity light_source_intensity = LightIntensity(source["intensity"][0].asFloat(),
                                                    source["intensity"][1].asFloat(),
                                                    source["intensity"][2].asFloat());
                                            
      
            


            sources.push_back(new LightSource(light_source_position, light_source_intensity));
        }
      
      const Json::Value& objects_data = scene_data["objects"];
      RootObject *root = nullptr;
      std::vector <BaseObject*> objects ;

      for(Json::Value::const_iterator it = objects_data.begin();it!=objects_data.end(); it++){
        const Json::Value& object = *it;

        // iterate over combinations:
        for(const auto& key : object.getMemberNames()){
          Combination *comb ;
          const Json::Value& comb_data = object[key];  // go to the first combination
          
          // iterate over the transformations:
          for(Json::Value::const_iterator it_comb = comb_data.begin(); it_comb!=comb_data.end(); it_comb++){
            const Json::Value& trans_data = *it_comb;
            BaseObject *objptr = nullptr;

            for(const auto& transkey : trans_data.getMemberNames()){
              Transformation *trans = nullptr ;
            
              
              const Json::Value& transformation = trans_data[transkey];
              const Json::Value& factors = transformation["factors"];
              const Json::Value& subject = transformation["subject"];

              // default black color:
              LightIntensity ambient_default(0.0f, 0.0f, 0.0f);
              LightIntensity diffuse_default(0.0f, 0.0f, 0.0f);
              LightIntensity specular_default(0.0f, 0.0f, 0.0f);
              LightIntensity reflected_default(0.0f, 0.0f, 0.0f);
              LightIntensity refracted_default(0.0f, 0.0f, 0.0f);
              ColData color_default(ambient_default,diffuse_default,specular_default,
                                    reflected_default, refracted_default,0);

              if(subject.isMember("sphere")){
                // check if colors are given:
                if(subject["color"].isNull()){
                  ColData sphere_sphere = color_default ;
                }  
                

                float radius = subject["sphere"]["radius"].asFloat();
                ColData sphere_color(LightIntensity(subject["sphere"]["color"]["ambient"][0].asFloat(),subject["sphere"]["color"]["ambient"][1].asFloat(),subject["sphere"]["color"]["ambient"][2].asFloat()),
                                      LightIntensity(subject["sphere"]["color"]["diffuse"][0].asFloat(),subject["sphere"]["color"]["diffuse"][1].asFloat(),subject["sphere"]["color"]["diffuse"][2].asFloat()),
                                      LightIntensity(subject["sphere"]["color"]["specular"][0].asFloat(),subject["sphere"]["color"]["specular"][1].asFloat(),subject["sphere"]["color"]["specular"][2].asFloat()),
                                      LightIntensity(subject["sphere"]["color"]["reflected"][0].asFloat(),subject["sphere"]["color"]["reflected"][1].asFloat(),subject["sphere"]["color"]["reflected"][2].asFloat()),
                                      LightIntensity(subject["sphere"]["color"]["refracted"][0].asFloat(),subject["sphere"]["color"]["refracted"][1].asFloat(),subject["sphere"]["color"]["refracted"][2].asFloat()),
                                      subject["sphere"]["color"]["shininess"].asFloat());

                BaseObject *objptr = new Sphere(sphere_color,subject["sphere"]["index"].asFloat());
                  


              }

              else if(subject.isMember("halfSpace")){
                // check if colors are given:
                if(subject["color"].isNull()){
                  ColData halfspace_color = color_default;
                  
                }  

                ColData halfspace_color(LightIntensity(subject["halfSpace"]["color"]["ambient"][0].asFloat(),subject["halfSpace"]["color"]["ambient"][1].asFloat(),subject["halfSpace"]["color"]["ambient"][2].asFloat()),
                                        LightIntensity(subject["halfSpace"]["color"]["diffuse"][0].asFloat(),subject["halfSpace"]["color"]["diffuse"][1].asFloat(),subject["halfSpace"]["color"]["diffuse"][2].asFloat()),
                                        LightIntensity(subject["halfSpace"]["color"]["specular"][0].asFloat(),subject["halfSpace"]["color"]["specular"][1].asFloat(),subject["halfSpace"]["color"]["specular"][2].asFloat()),
                                        LightIntensity(subject["halfSpace"]["color"]["reflected"][0].asFloat(),subject["halfSpace"]["color"]["reflected"][1].asFloat(),subject["halfSpace"]["color"]["reflected"][2].asFloat()),
                                        LightIntensity(subject["halfSpace"]["color"]["refracted"][0].asFloat(),subject["halfSpace"]["color"]["refracted"][1].asFloat(),subject["halfSpace"]["color"]["refracted"][2].asFloat()),
                                        subject["halfSpace"]["color"]["shininess"].asFloat());
                  
                BaseObject *objptr = new HalfSpace(halfspace_color, subject["halfSpace"]["index"].asFloat(), Eigen::Vector4f(subject["halfSpace"]["normal"][0].asFloat(),
                                                                                                                             subject["halfSpace"]["normal"][1].asFloat(),
                                                                                                                             subject["halfSpace"]["normal"][2].asFloat(),0));

              } 
              

              if(transformation.isMember("scaling")){
                trans = Transformation::Scaling(objptr, factors[0].asFloat(), factors[1].asFloat(), factors[2].asFloat());
                objects.push_back(trans);
                }

              if(transformation.isMember("translation")){
                trans = Transformation::Translation(objptr, factors[0].asFloat(), factors[1].asFloat(), factors[2].asFloat());
                objects.push_back(trans);

                }
              
              if(transformation.isMember("rotation")){
                if(transformation["rotation"]["direction"].asUInt() == 0){

                  trans = Transformation::Rotation_X(objptr,transformation["rotation"]["angle"].asFloat());

                }
                else if(transformation["rotation"]["direction"].asUInt() == 1){
                  trans = Transformation::Rotation_Y(objptr,transformation["rotation"]["angle"].asFloat());


                }
                else if(transformation["rotation"]["direction"].asUInt() == 3){
                  trans = Transformation::Rotation_Z(objptr,transformation["rotation"]["angle"].asFloat());

                

                }
              }
            }

            if(key=="union"){
              comb = new Union(objects);
            }

            else if(key=="intersection"){
              comb = new Intersection(objects);
            }

            else if(key=="exlusion"){
              comb = new Exclusion(objects);
            }

            else if(key=="subtraction"){
              comb = new Subtraction(objects);
            }

            root = new RootObject(comb);

            }
            
          }

    }

    return Scene(dpi, width, height,
                         position, observer, 
                         ambient_light, index,
                         max_recursion_depth,
                         sources, root);
          
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