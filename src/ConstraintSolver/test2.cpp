#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ConstraintSolver.h"
#include "ModelWorkspace.h"
#include "../mmcMatrix/mmcMatrix.h"

using namespace std;

int main(void)
{
	Point point1(0.0,0.0,0.0);
	Point point2(10.0,0.0,0.0);
	Point point3(10.0,10.0,0.0);
	Point point4(0.0,10.0,0.0);	

	Line line1(point1,point2);
	Line line2(point2,point3);
	Line line3(point3,point4);
	Line line4(point4,point1);

	DisplacementConstraint constraint1(point1,point2,11);

	return 0;
}

