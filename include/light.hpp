#pragma once

#include <iostream>
#include <array>
#include <Dense>

#include <custom_exceptions.hpp>
#include "defines.h"

/**
 * \class LightIntensity light.hpp
 * 
 * \brief Holds a color in RGB format.
 * 
 * Color values between 0 and 1 are used.
 */
class LightIntensity {
private:
  /**
   * \brief Consists of color values for the red, green and blue channel.
   */
  std::array<float, NUM_COL> rgb;

public:
  /**
   * \brief Base Constructor for LightIntensity.
   * 
   * Negative values are set to 0.
   * Values greater than 1 are capped at 1.
   * 
   * \param rgb array with rgb values
   */
  LightIntensity(std::array<float, NUM_COL> rgb);

  /**
   * \brief Constructor with seperate arguments for each channel.
   * 
   * Negative values are set to 0.
   * Values greater than 1 are capped at 1.
   * 
   * \param red value for red channel
   * \param green value for green channel
   * \param blue value for blue channel
   */
  LightIntensity(float red, float green, float blue);

  /**
   * \brief Default Constructor for LightIntensity.
   * 
   * Sets the color to black (0, 0, 0).
   */
  LightIntensity();

  /** \brief Constructs a LightIntensity with black color.*/
  inline static LightIntensity black() {
    return LightIntensity(0, 0, 0);
  }
  /** \brief Constructs a LightIntensity with silver color.*/
  inline static LightIntensity silver() {
    return LightIntensity(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
  }
  /** \brief Constructs a LightIntensity with gray color.*/
  inline static LightIntensity gray() {
    return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
  }
  /** \brief Constructs a LightIntensity with white color.*/
  inline static LightIntensity white() {
    return LightIntensity(1, 1, 1);
  }
  /** \brief Constructs a LightIntensity with maroon color.*/
  inline static LightIntensity maroon() {
    return LightIntensity(128.0 / 255.0, 0, 0);
  }
  /** \brief Constructs a LightIntensity with red color.*/
  inline static LightIntensity red() {
    return LightIntensity(1, 0, 0);
  }
  /** \brief Constructs a LightIntensity with purple color.*/
  inline static LightIntensity purple() {
    return LightIntensity(128.0 / 255.0, 0, 128.0 / 255.0);
  }
  /** \brief Constructs a LightIntensity with fuchsia color.*/
  inline static LightIntensity fuchsia() {
    return LightIntensity(1, 0, 1);
  }
  /** \brief Constructs a LightIntensity with green color.*/
  inline static LightIntensity green() {
    return LightIntensity(0, 128.0 / 255.0, 0);
  }
  /** \brief Constructs a LightIntensity with lime color.*/
  inline static LightIntensity lime() {
    return LightIntensity(0, 1, 0);
  }
  /** \brief Constructs a LightIntensity with olive color.*/
  inline static LightIntensity olive() {
    return LightIntensity(128.0 / 255.0, 128.0 / 255.0, 0);
  }
  /** \brief Constructs a LightIntensity with yellow color.*/
  inline static LightIntensity yellow() {
    return LightIntensity(1, 1, 0);
  }
  /** \brief Constructs a LightIntensity with navy color.*/
  inline static LightIntensity navy() {
    return LightIntensity(0, 0, 128.0 / 255.0);
  }
  /** \brief Constructs a LightIntensity with blue color.*/
  inline static LightIntensity blue() {
    return LightIntensity(0, 0, 1);
  }
  /** \brief Constructs a LightIntensity with teal color.*/
  inline static LightIntensity teal() {
    return LightIntensity(0, 128.0 / 255.0, 128.0 / 255.0);
  }
  /** \brief Constructs a LightIntensity with aqua color.*/
  inline static LightIntensity aqua() {
    return LightIntensity(0, 1, 1);
  }
  /** \brief Constructs a LightIntensity with gold color.*/
  inline static LightIntensity gold() {
    return LightIntensity(170.0 / 255.0, 127.0 / 255.0, 46.0 / 255.0);
  }

  /** 
   * \brief Getter function to access the individual color channels.
   * \param k color channel to get
  */
  const float& at(unsigned k) const;

  /**
   * \brief Inplace addition of two LightIntensity objects.
   * 
   * Performs addition channel-wise according to \f$1-((1-l_1)\cdot(1-l_2))\f$.
   * 
   * \param other Second LightIntensity class to add to *this.
   */
  void operator+=(const LightIntensity& other);
  /**
   * \brief Addition of two LightIntensity objects.
   * 
   * Performs addition according to the inplace addition LightIntensity::operator+=(const LightIntensity&).
   * 
   * \param l1 first LightIntensity to add
   * \param l2 second LightIntensity to add
   * 
   * \return Sum of l1 and l2.
   * 
   * \sa LightIntensity::operator+=
   */
  friend const LightIntensity operator+(LightIntensity l1, const LightIntensity& l2);
  
  /**
   * \brief Inplace multiplication of two LightIntensity objects.
   * 
   * Performs channel-wise multiplication.
   * 
   * \param other Second LightIntensity to multiply to *this.
   */
  void operator*=(const LightIntensity& other);
  /**
   * \brief Multiplication of two LightIntensity objects.
   * 
   * Performs multiplication according to the inplace multiplication LightIntensity::operator*=(const LightIntensity& other).
   * 
   * \param l1 first LightIntensity to multiply
   * \param l2 second LightIntensity to multiply
   * 
   * \return Product of l1 and l2.
   * 
   * \sa LightIntensity::operator*=(const LightIntensity& other)
   */
  friend const LightIntensity operator*(LightIntensity l1, const LightIntensity& l2);

  /**
   * \brief Inplace Multiplication with a scalar, floating point number.
   * 
   * Multiplies each channel with scalar.
   * For safty reasons, it is advised to only use scalar values smaller than 1,
   * though not enforced.
   * 
   * \param scalar scalar value to multiply with
   */
  void operator*=(float scalar);
  /**
   * \brief Multiplication of LightIntensity with a scalar, floating point number on the right.
   * 
   * Multiplies each channel with scalar, according to LightIntensity::operator*=(float).
   * 
   * \param li LightIntensity to be multiplied
   * \param scalar floating point value to multiply with
   * 
   * \return Channel-wise product of li with scalar.
   * 
   * \sa LightIntensity::operator*=(float)
   */
  friend const LightIntensity operator*(LightIntensity li, float scalar);
  /**
   * \brief Multiplication of LightIntensity with a scalar, floating point number on the left.
   * 
   * Multiplies each channel with scalar, according to LightIntensity::operator*=(float).
   * 
   * \param scalar floating point value to multiply with
   * \param li LightIntensity to be multiplied
   * 
   * \return Channel-wise product of li with scalar.
   * 
   * \sa LightIntensity::operator*=(float)
   */
  friend const LightIntensity operator*(float scalar, LightIntensity li);

  /**
   * \brief Formats LightIntensity color values into an output stream.
   * 
   * \param out outputstream to format into
   * \param li LightIntensity to format into out
   * 
   * \return modified output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const LightIntensity& li);
};

/**
 * \class ColData light.hpp
 * 
 * \brief Holds all data specifying the color (i.e. light interaction) of an object.
 */
struct ColData {
  /** 
   * \brief Corresponds to the Ambient Reflection Coefficient. 
   * Determines the illumination by ambient light.
  */
  LightIntensity ambient;

  /** 
   * \brief Corresponds to the Diffuse Reflection Coefficient.
   * Determines the illumination by Lambert's Law.
  */
  LightIntensity diffuse;

  /**
   * \brief Corresponds to the Specular Reflection Coefficient.
   * Determines the illumination by the direct reflection of the light to the observer.
  */
  LightIntensity specular;

  /**
   * \brief Corresponds to the Reflected Reflection Coefficient.
   * Determines the illumination by the reflected ray.
  */
  LightIntensity reflected;

  /**
   * \brief Corresponds to the Refracted Reflection Coefficient. 
   * Determines the illumination by the refracted ray.
  */
  LightIntensity refracted;

  /** 
   * \brief Corresponds to the Shininess Constant. 
   * Higher values mean a weaker / smaller specular effect. 
  */
  float shininess;

  /**
   * \brief Base Constructor for ColData.
   */
  ColData(LightIntensity ambient, LightIntensity diffuse, LightIntensity specular, LightIntensity reflected, LightIntensity refracted, float shininess);
  
  /**
   * \brief Default Constructor for ColData.
   * 
   * Initializes each LightIntensity to black and the shininess to 0.
   */
  ColData();
};

/**
 * \class LightSource light.hpp
 * 
 * \brief A light source which is localized in space.
 */
class LightSource {
private:
  /** \brief Position of the light source.*/
  Eigen::Vector4d position;

  /** \brief Color of the light source. */
  LightIntensity intensity;

public:
  /**
   * \brief Base Constructor for LightSource.
   */
  LightSource(const Eigen::Vector4d& position, const LightIntensity& intensity);

  /**
   * \brief Constructor for LightSource using a 3-dimensional vector.
   * 
   * Converts the position vector into a homogenous coordinate point vector 
   * (i.e. append a 1).
   */
  LightSource(const Eigen::Vector3d& position, const LightIntensity& intensity);

  /**
   * \brief Default Constructor for LightSource.
   * 
   * Initializes #position to the origin and #intensity to black.
   */
  LightSource();

  /**
   * \brief Destructor for LightSource.
   * Mainly used for debugging purposes.
   */
  ~LightSource();

  /**
   * \brief Getter function to retrieve color values.
   */
  const LightIntensity& rgb() const;

  /**
   * \brief Getter function to retrieve the position.
   */
  const Eigen::Vector4d& pos() const;
};