#include <composite.hpp>

BaseObject* Composites::Cube(ColData col, float index){
  HalfSpace* right = new HalfSpace(col, index, Eigen::Vector4d(1, 0, 0, 0));
  HalfSpace* up = new HalfSpace(col, index, Eigen::Vector4d(0, 1, 0, 0));
  HalfSpace* front = new HalfSpace(col, index, Eigen::Vector4d(0, 0, 1, 0));
  HalfSpace* back = new HalfSpace(col, index, Eigen::Vector4d(0, 0, -1, 0));
  HalfSpace* down = new HalfSpace(col, index, Eigen::Vector4d(0, -1, 0, 0));
  HalfSpace* left = new HalfSpace(col, index, Eigen::Vector4d(-1, 0, 0, 0));

  Transformation* right_trans = Transformation::Translation(right, 0.5, 0, 0);
  Transformation* up_trans = Transformation::Translation(up, 0, 0.5, 0);
  Transformation* front_trans = Transformation::Translation(front, 0, 0, 0.5);
  Transformation* back_trans = Transformation::Translation(back, 0, 0, -0.5);
  Transformation* down_trans = Transformation::Translation(down, 0, -0.5, 0);
  Transformation* left_trans = Transformation::Translation(left, -0.5, 0, 0);

  Intersection* cube = new Intersection({right_trans, up_trans, front_trans, back_trans, down_trans, left_trans});
  return cube;
}

BaseObject* Composites::Prism(ColData col, float index){
  HalfSpace* half1 = new HalfSpace(col, index, Eigen::Vector4d(-1, 0, 0, 0));
  Transformation* c = Transformation::Translation(half1, - 1 / (2 * sqrtf64(3)), 0, 0);

  HalfSpace* half2 = new HalfSpace(col, index, Eigen::Vector4d(0.5, - sqrtf64(3) / 2, 0, 0));
  Transformation* b = Transformation::Translation(half2, 1 / sqrtf64(3), 0, 0);

  HalfSpace* half3 = new HalfSpace(col, index, Eigen::Vector4d(0.5, sqrtf64(3) / 2, 0, 0));
  Transformation* a = Transformation::Translation(half3, 1 / sqrtf64(3), 0, 0);

  HalfSpace* half4 = new HalfSpace(col, index, Eigen::Vector4d(0, 0, -1, 0));
  Transformation* front = Transformation::Translation(half4, 0, 0, -0.5);

  HalfSpace* half5 = new HalfSpace(col, index, Eigen::Vector4d(0, 0, 1, 0));
  Transformation* back = Transformation::Translation(half5, 0, 0, 0.5);
  
  Intersection* prism = new Intersection({a, b, c, front, back});
  return prism;
}

BaseObject* Composites::Triforce(ColData col, float index){
  BaseObject* prism1 = Composites::Prism(col, index);
  Transformation* trans1 = Transformation::Translation(prism1, 1 / (2 * sqrtf64(3)), 0, 0);

  BaseObject* prism2 = Composites::Prism(col, index);
  Transformation* trans2 = Transformation::Translation(prism2, - 1 / sqrtf64(3), -0.5, 0);

  BaseObject* prism3 = Composites::Prism(col, index);
  Transformation* trans3 = Transformation::Translation(prism3, - 1 / sqrtf64(3), 0.5, 0);

  Union* tri = new Union({trans1, trans2, trans3});
  return tri;
}
