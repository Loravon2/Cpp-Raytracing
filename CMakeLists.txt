cmake_minimum_required(VERSION 3.14)

project(cpp_raytracing VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/defines)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()


# credit to https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
option(BUILD_DOC "Build documentation" ON)

if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
  find_program(BROWSER_DIR NAMES firefox firefox-developer-edition Firefox Firefox-Developer-Edition chrome Chrome chromium Chromium edge Edge brave Brave opera Opera)

  find_package(Doxygen QUIET)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/resources/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    add_custom_target(documentation
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generating documentation with Doxygen"
                      VERBATIM)

    if(NOT BROWSER_DIR EQUAL "BROWSER_DIR-NOTFOUND")
      add_custom_target(open_doc
                        COMMAND ${BROWSER_DIR} ${CMAKE_CURRENT_BINARY_DIR}/doc_doxygen/html/index.html
                        DEPENDS documentation
                        COMMENT "Opening documentation with ${BROWSER_DIR}")
    endif(NOT BROWSER_DIR EQUAL "BROWSER_DIR-NOTFOUND")
  else(DOXYGEN_FOUND)
    message("Doxygen must be installed to generate the documentation")
  endif(DOXYGEN_FOUND)
endif()

option(custom_assert "using custom-defined assert" 1)
if(custom_assert)
  add_compile_definitions(-DACTIVATE_CUSTOM_ASSERT)
endif()


include(CTest)
include(FetchContent)


add_compile_options(-Wall -Wextra -Wpedantic)


find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 4.90 QUIET)
find_package(nlohmann_json 3.11.3 QUIET)

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

include_directories(${INC_DIR}
                    ${DEF_DIR}
                    ${OpenCV_INCLUDE_DIRS}
                    ${Eigen3_SOURCE_DIR}/Eigen
                    ${nlohmann_json_SOURCE_DIR}/single_include/nlohmann)

file(GLOB sourceFiles CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)
add_library(Cpp-Raytracing ${sourceFiles})


add_executable(Cpp-Raytracing.exe main.cpp)

target_compile_definitions(Cpp-Raytracing.exe PRIVATE
                           $<$<CONFIG:Debug>:DEBUG>
)

target_compile_options(Cpp-Raytracing.exe PRIVATE
                       $<$<CONFIG:Debug>:-g>)

target_compile_definitions(Cpp-Raytracing PRIVATE
                           $<$<CONFIG:Debug>:DEBUG>
)

target_compile_options(Cpp-Raytracing PRIVATE
                       $<$<CONFIG:Debug>:-g>)

add_custom_target(run
                  COMMAND Cpp-Raytracing.exe
                  DEPENDS Cpp-Raytracing.exe)


target_link_libraries(Cpp-Raytracing.exe 
                      opencv_core
                      opencv_imgcodecs
                      Cpp-Raytracing)




add_executable(unit_tests tests/unit_tests.cpp)

target_link_libraries(unit_tests 
                      opencv_core
                      opencv_imgcodecs
                      Cpp-Raytracing)

add_test(NAME unit_tests
         COMMAND unit_tests)


add_executable(integration_tests tests/integration_tests.cpp)

target_link_libraries(integration_tests 
                      opencv_core
                      opencv_imgcodecs
                      Cpp-Raytracing)

add_test(NAME integration_tests
         COMMAND integration_tests)
                      

add_executable(end_to_end_tests tests/end_to_end_tests.cpp)

target_link_libraries(end_to_end_tests 
                      opencv_core
                      opencv_imgcodecs
                      Cpp-Raytracing)

add_test(NAME end_to_end_tests
         COMMAND end_to_end_tests)