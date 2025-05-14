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
#include <composite.hpp>
#include "defines.h"

/**
 * \class Scene scene.hpp
 * 
 * \brief Main class that serves as an interface between user and scene.
 * 
 * Can be created by reading a describing .json file or independently by the user.
 * Can generate an image of the scene in form of an opencv matrix.
 */
class Scene {
private:
  /** \brief Function pointer to object creating helper functions for reading the scene. */
  typedef BaseObject* (*action_t)(nlohmann::json&);
  /** \brief Function pointer to rotation creating functions. */
  typedef Transformation* (*rotation_t)(BaseObject*, double);
  /** \brief Action handler to choose reading helper function based on object string in the scene describing .json. */
  static const std::map<std::string, action_t> action_handler;
  /** \brief Action handler to choose the right rotation based on an index 
   * 
   * 0 -- X-Axis, 1 -- Y-Axis, 2 -- Z-Axis
  */
  static const std::array<rotation_t, 3> rotation_handler;
  /** 
   * \brief Action handler to choose the right color based on the read string.
   */
  static const std::map<std::string, LightIntensity> color_handler;

  /** \brief Helper function to read a LightSource from .json */
  static LightSource* read_source(nlohmann::json& descr);
  /** \brief Helper function to read a LightIntensity from .json */
  static LightIntensity read_color(nlohmann::json& descr);

  /** \brief Helper function to read a ColData from .json */
  static ColData read_col_data(nlohmann::json& descr);
  /** \brief Helper function to read a list of BaseObjects from .json into a vector. */
  static std::vector<BaseObject*> read_obj_list(nlohmann::json& descr);

  /** \brief Helper function to read a Sphere from .json */
  static BaseObject* read_sphere(nlohmann::json& descr);
  /** \brief Helper function to read a HalfSpace from .json */
  static BaseObject* read_half_space(nlohmann::json& descr);
  /** \brief Helper function to read a Cylinder from .json */
  static BaseObject* read_cylinder(nlohmann::json& descr);

  /** \brief Helper function to read a scaling Transformation from .json */
  static BaseObject* read_scaling(nlohmann::json& descr);
  /** \brief Helper function to read a rotation Transformation from .json */
  static BaseObject* read_rotation(nlohmann::json& descr);
  /** \brief Helper function to read a translation Transformation from .json */
  static BaseObject* read_translation(nlohmann::json& descr);

  /** \brief Helper function to read a Union from .json */
  static BaseObject* read_union(nlohmann::json& descr);
  /** \brief Helper function to read an Intersection from .json */
  static BaseObject* read_intersection(nlohmann::json& descr);
  /** \brief Helper function to read an Exclusion from .json */
  static BaseObject* read_exclusion(nlohmann::json& descr);
  /** \brief Helper function to read a Subtraction from .json */
  static BaseObject* read_subtraction(nlohmann::json& descr);

  /** \brief Helper function to read a Cube from .json */
  static BaseObject* read_cube(nlohmann::json& descr);
  /** \brief Helper function to read a Prism from .json */
  static BaseObject* read_prism(nlohmann::json& descr);
  /** \brief Helper function to read a Triforce from .json */
  static BaseObject* read_triforce(nlohmann::json& descr);

  /** \brief Prints the current progress as a progression bar in std::cout. 
   * 
   * Credit goes to leemes on stackoverflow for the implementation.
   * 
   * https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
  */
  static void progress_bar(float progress);


  float dpi; //!< pixels per unit length in the final image
  float L_x; //!< size of the screen in x-direction (vertically)
  float L_y; //!< size of the screen in y-direction (horizontally)

  Eigen::Vector4d position; //!< position of the bottom left corner of the screen
  Eigen::Vector4d observer; //!< position of the camera / observer

  LightIntensity ambient_light; //!< amount of ambient light in the scene
  float global_index; //!< refraction index to be used, when a ray is in no object
  std::map<float, unsigned> object_indexs;

  unsigned max_recursion_depth;

  std::vector<LightSource*> sources; //!< list of all LightSources in the scene
  RootObject* objects; //!< The root of the scene. All interaction with the scenes objects goes through this.
  
public:
  /**
   * \brief Constructs a Scene based on an input description.
   * 
   * \param input stream with description in json format
   * 
   * \returns Constructed Scene according to input.
   */
  static Scene read_parameters(std::istream& input);
  
  /** 
   * \brief Base Constructor for Scene.
   */
  Scene(float dpi, float L_x, float L_y,
        Eigen::Vector4d position, Eigen::Vector4d observer,
        LightIntensity ambient_light, float global_index,
        unsigned max_recursion_depth,
        std::vector<LightSource*> sources, RootObject* objects);
  /**
   * \brief Default Constructor for Scene.
   */
  Scene();

  /**
   * \brief Destructor for the whole Scene
   * 
   * Frees each LightSource in #sources and through #objects all BaseObjects in the scene.
   */
  ~Scene();

  /**
   * \brief Traces a single ray in the Scene.
   * 
   * \param ray The Ray to trace.
   * \param depth The current recursion depth. No more reflactions and refractions are calculated, when depth = #max_recursion_depth.
   * 
   * \returns The LightIntensity perceived by the ray.
   */
  LightIntensity trace_ray(const Ray& ray, unsigned depth);
  /**
   * \brief Generates an image of the Scene.
   * 
   * Traces Rays through every pixel of the screen.
   * 
   * \returns An opencv matrix consisting of the generated image. Can be written into an actual image by cv::imwrite. 
   */
  cv::Mat_<cv::Vec3b> generate();
};