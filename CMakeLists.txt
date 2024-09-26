cmake_minimum_required(VERSION 3.24)

project(cpp_raytracing VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)


include(FetchContent)

FetchContent_Declare(
  OpenCV
  GIT_REPOSITORY  https://github.com/opencv/opencv.git
  GIT_TAG         71d3237a093b60a27601c20e9ee6c3e52154e8b1
  FIND_PACKAGE_ARGS NAMES OpenCV
)

FetchContent_Declare(
  Eigen3
  GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
  GIT_TAG         2a3465102ac48d5e71b134d77b25665d63120eff
  FIND_PACKAGE_ARGS NAMES Eigen3
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY  https://github.com/nlohmann/json.git
  GIT_TAG         9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03
  FIND_PACKAGE_ARGS NAMES nlohmann_json
)

FetchContent_MakeAvailable(OpenCV Eigen3 nlohmann_json)


add_executable(main main.cpp ${SRC_DIR}/composite.cpp ${SRC_DIR}/file_input.cpp ${SRC_DIR}/light.cpp ${SRC_DIR}/objects.cpp ${SRC_DIR}/ray.cpp ${SRC_DIR}/scene.cpp)


target_include_directories(main PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/defines
                           ${OpenCV_SOURCE_DIR}
                           ${Eigen3_SOURCE_DIR}/Eigen
                           ${nlohmann_json_SOURCE_DIR}/single_include/nlohmann)

target_link_libraries(main 
                      opencv_core
                      opencv_imgcodecs)