cmake_minimum_required(VERSION 3.14)

project(cpp_raytracing VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)


find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 4.90 QUIET)
find_package(nlohmann_json 3.11.3 QUIET)

include(FetchContent)

if(NOT Eigen3_FOUND)
  FetchContent_Declare(
    Eigen3
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG         2a3465102ac48d5e71b134d77b25665d63120eff
  )

  FetchContent_MakeAvailable(Eigen3)
endif()

if (NOT nlohmann_json_FOUND)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03
  )

  FetchContent_MakeAvailable(nlohmann_json)
endif()

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