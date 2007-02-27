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
	Point point1(0.0,0.0,0.0,false,false,false);  // none of the dof's can vary
	Point point2(10.0,0.0,0.0,true,false,false);  // only x dof can vary
	Point point3(10.0,10.0,0.0,true,true,false);  // x and y dof's can vary
	Point point4(0.0,10.0,0.0,false,true,false);  // only y dof can vary	

	Line line1(point1,point2);
	Line line2(point2,point3);
	Line line3(point3,point4);
	Line line4(point4,point1);

	// These 4 constraints will fully constrain the four free DOF's defined about
	DistanceConstraint constraint1(point1,point2,11);
	DistanceConstraint constraint2(point2,point3,12);
	ParallelConstraint constraint3(line1,line3);
	ParallelConstraint constraint4(line2,line4);

	return 0;
}

