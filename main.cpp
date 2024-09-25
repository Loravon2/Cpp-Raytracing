#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

#include <scene.hpp>
#include "defines.h"

int main() {
  std::cout << "Hello. To load one of our example scenes, please give a number from 1 to "
  << NUM_EXAMPLES << ".\nIf you want load a custom scene, please give the file path of your json file"
  " relative to the programs top level directory (Cpp-Raytracing/).\nFor more infos regarding the format"
  " of the json file, see our documentation." << std::endl;

  std::cout << "\nfile path / example(1-" << NUM_EXAMPLES << "): " << std::flush;

  std::string file_path;
  std::cin >> file_path;

  try {
    int example_code = std::stoi(file_path);
    if (1 <= example_code and example_code <= NUM_EXAMPLES) {
      file_path = std::string() + "examples/example" + file_path + ".json";
    }
  }
  catch (std::invalid_argument&) {}

  std::string final_path = std::string() + "../" + file_path;

  std::ifstream inp;
  inp.open(final_path, std::ios::in);

  if (not inp.is_open()) {
    std::cout << "The given input file " << final_path << " could not be found.\nMake sure, that you gave"
    " the file path relative to the programs top level directory.\nIf you are trying to use the given examples, make sure"
    " that the contents of the examples directory was not changed.\n\nNow exiting." << std::endl;
    return 1;
  }

  Scene scene = Scene::read_parameters(inp);

  std::cout << "\nThe scene was loaded successfully.\nDepending on size and resolution, the rendering may"
  " take a while.\nDo you want to continue? (y/N)" << std::endl;

  char answ;
  std::cin >> answ;

  if (answ != 'y' and answ != 'Y') {
    std::cout << "Aborting." << std::endl;
    return 0;
  }

  std::cout << "\nStarting rendering process" << std::endl;

  cv::Mat_<cv::Vec3b> img = scene.generate();

  cv::imwrite("output.png", img);

  std::cout << "\nRendering was successfull. The final image can be found as \"output.png\" in your build directory."
  << std::endl;

  return 0;
}