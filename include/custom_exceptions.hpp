#pragma once

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

class Cpp_Raytracing_Exception: public std::exception {
public:
  virtual const char* what() const noexcept override;
};

class Cpp_Raytracing_ASSERTION_FAILED: public Cpp_Raytracing_Exception {
private:
  char* info;

public:
  Cpp_Raytracing_ASSERTION_FAILED(const char*, int);
  ~Cpp_Raytracing_ASSERTION_FAILED();

  virtual const char* what() const noexcept override;
};

#ifdef ACTIVATE_CUSTOM_ASSERT
  #define CUSTOM_ASSERT(asrt) custom_assert(asrt, __FILE__, __LINE__)

  template <typename T>
  inline void custom_assert(T&& assertion, const char* file, int line) {
    if (!assertion) {
      throw Cpp_Raytracing_ASSERTION_FAILED(file, line);
    }
  }
#else
  #define CUSTOM_ASSERT(asrt)
#endif