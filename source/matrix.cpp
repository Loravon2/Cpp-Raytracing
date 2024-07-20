#include <matrix.hpp>

TransformMatrix::TransformMatrix(std::array <std::array <double, 4>, 4> mat): content(mat) {}
TransformMatrix::TransformMatrix(std::array <double, 4> a, std::array <double, 4> b, std::array <double, 4> c, std::array <double, 4> d): TransformMatrix(std::array <std::array <double, 4>, 4>()) {
  for (size_t i = 0; i < 4; i++) {
    this->at(0, i) = a.at(i);
    this->at(1, i) = b.at(i);
    this->at(2, i) = c.at(i);
    this->at(3, i) = d.at(i);
  }
}
TransformMatrix::TransformMatrix(): TransformMatrix(std::array <std::array <double, 4>, 4>()) {
  for (size_t i = 0; i < 4; i++) {
    this->at(i, i) = 1;
  }
}

void TransformMatrix::inverse() {
  /* ERROR -- can only compute inverse of special matrix classes */
}

const double& TransformMatrix::at(size_t i, size_t j) const {
  return content.at(i).at(j);
}

double& TransformMatrix::at(size_t i, size_t j) {
  return content.at(i).at(j);
}

const Vector4D TransformMatrix::at(size_t i) const {
  Vector4D v(this->at(i, 0), this->at(i, 1), this->at(i, 2), this->at(i, 3));
  return v;
}

void TransformMatrix::operator*=(const TransformMatrix& other) {
  TransformMatrix copy = *this;

  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      this->at(i, j) = 0;
      for (size_t k = 0; k < 4; k++) {
        this->at(i, j) += copy.at(k, j) * other.at(i, k);
      }
    }
  }
}

std::ostream& operator<<(std::ostream& stream, const TransformMatrix& A) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      stream << A.at(j, i) << " ";
    }
    stream << "\n";
  } 

  return stream;
}

const Vector4D operator*(const TransformMatrix& A, const Vector4D& v) {
  std::array <double, 4> res;
  for (size_t i = 0; i < 4; i++) {
    res[i] = A.at(0, i) * v.x() + A.at(1, i) * v.y() + A.at(2, i) * v.z() + A.at(3, i) * v.pointVector();
  }

  return Vector4D(res);
}

const TransformMatrix operator*(TransformMatrix A, const TransformMatrix& B) {
  A *= B;
  return A;
}


ScalingMatrix::ScalingMatrix(double ax, double ay, double az): TransformMatrix({ax, 0, 0, 0}, {0, ay, 0, 0}, {0, 0, az, 0}, {0, 0, 0, 1}), ax(ax), ay(ay), az(az) {}
ScalingMatrix::ScalingMatrix(): ScalingMatrix(1, 1, 1) {}

void ScalingMatrix::inverse() {
  *this = ScalingMatrix(1.0 / ax, 1.0 / ay, 1.0 / az);
}

TranslationMatrix::TranslationMatrix(double tx, double ty, double tz): TransformMatrix({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {tx, ty, tz, 1}), tx(tx), ty(ty), tz(tz) {}
TranslationMatrix::TranslationMatrix(): TranslationMatrix(0, 0, 0) {}

void TranslationMatrix::inverse() {
  *this = TranslationMatrix(-tx, -ty, -tz);
}

RotationMatrix::RotationMatrix(double theta, axis_t i): TransformMatrix(), theta(theta), axis(i) {
  switch (i) {
    case X_AXIS: {
      this->at(1, 1) = cos(theta);
      this->at(1, 2) = sin(theta);
      this->at(2, 1) = -sin(theta);
      this->at(2, 2) = cos(theta);
      break;
    }
    case Y_AXIS: {
      this->at(0, 0) = cos(theta);
      this->at(0, 2) = sin(theta);
      this->at(2, 0) = -sin(theta);
      this->at(2, 2) = cos(theta);
      break;
    }
    case Z_AXIS: {
      this->at(0, 0) = cos(theta);
      this->at(0, 1) = sin(theta);
      this->at(1, 0) = -sin(theta);
      this->at(1, 1) = cos(theta);
      break;
    }
    default: {
      /* ERROR HANDLING -- invalid Axis */
    }
  }
}
RotationMatrix::RotationMatrix(): RotationMatrix(0, X_AXIS) {}

void RotationMatrix::inverse() {
  *this = RotationMatrix(-theta, axis);
}