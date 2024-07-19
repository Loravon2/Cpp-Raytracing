#include <matrix.hpp>

TransformMatrix::TransformMatrix(std::array <std::array <double, 4>, 4> mat): content(mat) {}
TransformMatrix::TransformMatrix(): TransformMatrix(std::array <std::array <double, 4>, 4>()) {
  for (size_t i = 0; i < 4; i++) {
    this->at(i, i) = 1;
  }
}
