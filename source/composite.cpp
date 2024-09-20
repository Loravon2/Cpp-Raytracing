#include <composite.hpp>

BaseObject* Composites::cube(ColData col, float index){

  HalfSpace* right = new HalfSpace(col, index, Eigen::Vector4f(1, 0, 0, 0));
  HalfSpace* up = new HalfSpace(col, index, Eigen::Vector4f(0, 1, 0, 0));
  HalfSpace* front = new HalfSpace(col, index, Eigen::Vector4f(0, 0, 1, 0));
  HalfSpace* back = new HalfSpace(col, index, Eigen::Vector4f(0, 0, -1, 0));
  HalfSpace* down = new HalfSpace(col, index, Eigen::Vector4f(0, -1, 0, 0));
  HalfSpace* left = new HalfSpace(col, index, Eigen::Vector4f(-1, 0, 0, 0));

  Transformation* RightTrans = Transformation::Translation(right, 0.5, 0, 0);
  Transformation* UpTrans = Transformation::Translation(up, 0, 0.5, 0);
  Transformation* FrontTrans = Transformation::Translation(front, 0, 0, 0.5);
  Transformation* BackTrans = Transformation::Translation(back, 0, 0, -0.5);
  Transformation* DownTrans = Transformation::Translation(down, 0, -0.5, 0);
  Transformation* LeftTrans = Transformation::Translation(left, -0.5, 0, 0);

  Intersection * Cube = new Intersection({RightTrans, UpTrans, FrontTrans, BackTrans, DownTrans, LeftTrans});
  return Cube;
}