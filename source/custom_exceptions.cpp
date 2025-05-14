#include <custom_exceptions.hpp>


const char* Cpp_Raytracing_Exception::what() const noexcept {
  return "ERROR : Some error within Cpp-Raytracing was detected.";
}

Cpp_Raytracing_ASSERTION_FAILED::Cpp_Raytracing_ASSERTION_FAILED(const char* file, int line) {
  info = (char*) malloc((45 + strlen(file) + line / 10) * sizeof(char));
  snprintf(info, (45 + strlen(file) + line / 10) * sizeof(char), "ERROR : FAILED ASSERTION in file %s and line %d.", file, line);
}

Cpp_Raytracing_ASSERTION_FAILED::~Cpp_Raytracing_ASSERTION_FAILED() {
  free(info);
}

const char* Cpp_Raytracing_ASSERTION_FAILED::what() const noexcept {  
  return info;
}
