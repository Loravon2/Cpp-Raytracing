#pragma once

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

/**
 * \class Cpp_Raytracing_Exception custom_exceptions.hpp
 * 
 * \brief Base class for all Cpp-Raytracing exceptions.
 * 
 * This class serves as a base class for all exceptions thrown by Cpp-Raytracing.
 */
class Cpp_Raytracing_Exception: public std::exception {
public:
  /**
   * \brief Informations about encountered error.
   * 
   * \returns C-String describing the encountered error.
   */
  virtual const char* what() const noexcept override;
};

/**
 * \class Cpp_Raytracing_ASSERTION_FAILED custom_exceptions.hpp
 * 
 * \brief Exception thrown by a failed assertion.
 * 
 * This exception is thrown, when an assertion in Cpp-Raytracing (precisely CUSTOM_ASSERTION) fails.
 */
class Cpp_Raytracing_ASSERTION_FAILED: public Cpp_Raytracing_Exception {
private:
  /**
   * \brief Contains the C-String with infos error.
   * 
   * Especially specifies the file and line where the assertion failed.
   * \sa what()
   */
  char* info;

public:
  /**
   * \brief Constructor taking information about where the assertion failed.
   * 
   * \param file name of the file, where the assertion failed 
   * \param line line number, where the assertion failed
   */
  Cpp_Raytracing_ASSERTION_FAILED(const char* file, int line);

  /**
   * \brief Destructor to free allocated memory of #info.
   * 
   * \sa info
   */
  ~Cpp_Raytracing_ASSERTION_FAILED();

  /**
   * \brief Informations about encountered error.
   * 
   * \returns C-String describing the encountered error.
   */
  virtual const char* what() const noexcept override;
};

#ifdef ACTIVATE_CUSTOM_ASSERT
  /**
   * \brief Helper Macro to retrieve file and line of assertion.
   * 
   * \param asrt value to be asserted, must be evaluable to a boolean
   */
  #define CUSTOM_ASSERT(asrt) custom_assert(asrt, __FILE__, __LINE__)

  /**
   * \brief Custom assertion to go around cmake deactivating assert.
   * 
   * Throws #Cpp_Raytracing_ASSERTION_FAILED if assertion is evaluated to false.
   * 
   * \tparam assertion value to be asserted, must be evaluable to a boolean
   * \param file name of the file, where the assertion occurs
   * \param line line where the assertion occurs
   */
  template <typename T>
  inline void custom_assert(T&& assertion, const char* file, int line) {
    if (!assertion) {
      throw Cpp_Raytracing_ASSERTION_FAILED(file, line);
    }
  }
#else
  #define CUSTOM_ASSERT(asrt)
#endif