#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ConstraintSolver.h"
#include "Ark3DModel.h"
#include "../mmcMatrix/mmcMatrix.h"

using namespace std;

int main(void)
{
	PointPointer point1(new Point(0.0,0.0,0.0,false,false,false));  // none of the dof's can vary
	PointPointer point2(new Point(10.0,0.0,0.0,true,false,false));  // only x dof can vary
	PointPointer point3(new Point(10.0,10.0,0.0,true,true,false));  // x and y dof's can vary
	PointPointer point4(new Point(0.0,10.0,0.0,false,true,false));  // only y dof can vary	

	LinePointer line1(new Line(point1,point2));
	LinePointer line2(new Line(point2,point3));
	LinePointer line3(new Line(point3,point4));
	LinePointer line4(new Line(point4,point1));

	// These 4 constraints will fully constrain the four free DOF's defined about
	DistanceConstraintPointer constraint1(new DistanceConstraint(point1,point2,11));
	DistanceConstraintPointer constraint2(new DistanceConstraint(point2,point3,12));
	ParallelConstraintPointer constraint3(new ParallelConstraint(line1,line3));
	ParallelConstraintPointer constraint4(new ParallelConstraint(line2,line4));

	// Create the ark 3d model object
	Ark3DModel my_model;
	
	// Add the primitives to the 3d model object
	my_model.AddPrimitive(point1);
	my_model.AddPrimitive(point2);
	my_model.AddPrimitive(point3);
	my_model.AddPrimitive(point4);
	my_model.AddPrimitive(line1);
	my_model.AddPrimitive(line2);
	my_model.AddPrimitive(line3);
	my_model.AddPrimitive(line4);

	// Add the constraints to the 3d model object
	my_model.AddConstraintEquation(constraint1);
	my_model.AddConstraintEquation(constraint2);
	my_model.AddConstraintEquation(constraint3);
	my_model.AddConstraintEquation(constraint4);

	// solve the constraint equations
	my_model.SolveConstraints();

	return 0;
}

