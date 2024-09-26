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

BaseObject* Composites::triangle(ColData col, float index){
   Intersection* cube1 = Composites::cube(col, index);
   Intersection* cube2 = Composites::cube(col, index);
   Intersection* cube3 = Composites::cube(col, index);
   Intersection* cube4 = Composites::cube(col, index);
   Intersection* cube5 = Composites::cube(col, index);
   Intersection* cube6 = Composites::cube(col, index);

   Transformation* RightTrans = Transformation::Translation(cube2, 0.5, 0, 0);
   Transformation* RightTrans2 = Transformation::Translation(cube3, 1, 0, 0);
   Transformation* UpTrans = Transformation::Translation(cube4, 0, 0.5, 0);
   Transformation* UpRTrans = Transformation::Translation(cube5, 0.5, 0.5, 0);
   Transformation* UpTrans2 = Transformation::Translation(cube6, 0, 1, 0);
   Union* tri = new Union({cube1, RightTrans, RightTrans2, UpTrans, UpRTrans, UpTrans2});
   return tri;
}