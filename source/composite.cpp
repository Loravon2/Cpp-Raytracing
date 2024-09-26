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

  Intersection * cube = new Intersection({right_trans, up_trans, front_trans, back_trans, down_trans, left_trans});
  return cube;
}


BaseObject* Composites::Cube_Pattern(ColData col, float index){
  BaseObject* cube = Composites::Cube(col, index);
  HalfSpace* diag1 = new HalfSpace(col, index, Eigen::Vector4d(1, 0, 0, 0));
  HalfSpace* diag2 = new HalfSpace(col, index, Eigen::Vector4d(-1, 0, 0, 0));
  
  Transformation* diag1Trans = Transformation::Rotation_Z(diag1, -M_PI_4);
  Transformation* diag2Trans = Transformation::Rotation_Z(diag2, M_PI_4);

  Union* uni = new Union({cube, diag1Trans, diag2Trans});
  return uni;
}


BaseObject* Composites::Triangle_Isosceles(ColData col, float index){
  BaseObject* cube1 = Composites::Cube(col, index);
  Transformation* cubetrans = Transformation::Rotation_Z(cube1, M_PI_4);
  HalfSpace* diag = new HalfSpace(col, index, Eigen::Vector4d(0, 1, 0, 0));
  
  Subtraction* tri = new Subtraction({cubetrans, diag});
  return tri;
}

//Dont know about this one. doesnt really work how i imagined it (i think?)
BaseObject* Composites::Triangle_Equilateral(ColData col, float index){
  BaseObject* cube = Composites::Cube(col, index);
  HalfSpace* diag1 = new HalfSpace(col, index, Eigen::Vector4d(1, 0, 0, 0));
  HalfSpace* diag2 = new HalfSpace(col, index, Eigen::Vector4d(-1, 0, 0, 0));
  
  Transformation* diag1Trans = Transformation::Rotation_Z(diag1, -M_PI_4);
  Transformation* diag2Trans = Transformation::Rotation_Z(diag2, M_PI_4);

  Subtraction* sub = new Subtraction({cube, diag1Trans, diag2Trans});
  return sub;
}

BaseObject* Composites::Triforce(ColData col, float index){
  BaseObject* triangle = Composites::Triangle_Isosceles(col, index);

  BaseObject* triangle2 = Composites::Triangle_Isosceles(col, index);
  Transformation* tritrans = Transformation::Translation(triangle2, -0.65, -0.65, 0);

  BaseObject* triangle3 = Composites::Triangle_Isosceles(col, index);
  Transformation* tritrans2 = Transformation::Translation(triangle3, 0.65, -0.65, 0);

  Union* uni = new Union({triangle, tritrans, tritrans2});
  return uni;
}
