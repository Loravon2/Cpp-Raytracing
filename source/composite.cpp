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

// Did union instead of subtraction but its an interesting pattern

Union* Composites::cube_pattern(ColData col, float index){
  Intersection* cube = Composites::cube(col, index);
  HalfSpace* diag1 = new HalfSpace(col, index, Eigen::Vector4f(1, 0, 0, 0));
  HalfSpace* diag2 = new HalfSpace(col, index, Eigen::Vector4f(-1, 0, 0, 0));
  
  Transformation* diag1Trans = Transformation::Rotation_Z(diag1, -M_PI_4);
  Transformation* diag2Trans = Transformation::Rotation_Z(diag2, M_PI_4);

  Union* uni = new Union({cube, diag1Trans, diag2Trans});
  return uni;
}


Subtraction* Composites::triangle_isosceles(ColData col, float index){
   Intersection* cube1 = Composites::cube(col, index);
   Transformation* cubetrans = Transformation::Rotation_Z(cube1, M_PI_4);
   HalfSpace* diag = new HalfSpace(col, index, Eigen::Vector4f(0, 1, 0, 0));
   
   Subtraction* tri = new Subtraction({cubetrans, diag});
   return tri;
}

//Dont know about this one. doesnt really work how i imagined it (i think?)
Subtraction* Composites::triangle_equilateral(ColData col, float index){
  Intersection* cube = Composites::cube(col, index);
  HalfSpace* diag1 = new HalfSpace(col, index, Eigen::Vector4f(1, 0, 0, 0));
  HalfSpace* diag2 = new HalfSpace(col, index, Eigen::Vector4f(-1, 0, 0, 0));
  
  Transformation* diag1Trans = Transformation::Rotation_Z(diag1, -M_PI_4);
  Transformation* diag2Trans = Transformation::Rotation_Z(diag2, M_PI_4);

  Subtraction* sub = new Subtraction({cube, diag1Trans, diag2Trans});
  return sub;
}
