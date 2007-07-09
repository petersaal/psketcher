#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../OpenCascadeBinding/OccPrimitives.h"
#include "../ConstraintSolver/Ark3DModel.h"
#include "../mmcMatrix/mmcMatrix.h"

using namespace std;

void constraint_solver_test(Handle(AIS_InteractiveContext) ais_context)
{
	PointPointer point1(new Point(0.0,0.0,0.0,false,false,false));  // none of the dof's can vary
	PointPointer point2(new Point(10.0,0.0,0.0,true,false,false));  // only x dof can vary
	PointPointer point3(new Point(10.0,10.0,0.0,true,true,false));  // x and y dof's can vary
	PointPointer point4(new Point(0.0,10.0,0.0,false,true,false));  // only y dof can vary	

	LinePointer line1(new Line(point1,point2));
	LinePointer line2(new Line(point2,point3));
	LinePointer line3(new Line(point3,point4));
	LinePointer line4(new Line(point4,point1));

	// create the AIS interactive objects using the ark3d OpenCascade binding
	OccLine oc_line1(line1,ais_context);
	OccLine oc_line2(line2,ais_context);
	OccLine oc_line3(line3,ais_context);
	OccLine oc_line4(line4,ais_context);

	OccPoint oc_point1(point1,ais_context);
	OccPoint oc_point2(point2,ais_context);
	OccPoint oc_point3(point3,ais_context);
	OccPoint oc_point4(point4,ais_context);

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

	// check some of the DOF's before the solve step
	cout << "Point 1 (x,y,z): (" << point1->GetXDOF()->GetValue() << ", " << point1->GetYDOF()->GetValue() << ", " << point1->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 2 (x,y,z): (" << point2->GetXDOF()->GetValue() << ", " << point2->GetYDOF()->GetValue() << ", " << point2->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 3 (x,y,z): (" << point3->GetXDOF()->GetValue() << ", " << point3->GetYDOF()->GetValue() << ", " << point3->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 4 (x,y,z): (" << point4->GetXDOF()->GetValue() << ", " << point4->GetYDOF()->GetValue() << ", " << point4->GetZDOF()->GetValue() << ")" << endl;

	// solve the constraint equations
	my_model.SolveConstraints();

	// check some of the DOF's to see what happened during the solve step 
	cout << "Point 1 (x,y,z): (" << point1->GetXDOF()->GetValue() << ", " << point1->GetYDOF()->GetValue() << ", " << point1->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 2 (x,y,z): (" << point2->GetXDOF()->GetValue() << ", " << point2->GetYDOF()->GetValue() << ", " << point2->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 3 (x,y,z): (" << point3->GetXDOF()->GetValue() << ", " << point3->GetYDOF()->GetValue() << ", " << point3->GetZDOF()->GetValue() << ")" << endl;
	cout << "Point 4 (x,y,z): (" << point4->GetXDOF()->GetValue() << ", " << point4->GetYDOF()->GetValue() << ", " << point4->GetZDOF()->GetValue() << ")" << endl;

}

// test the constraint solver with the 2D point and line primitives
void constraint_solver_test_2(Handle(AIS_InteractiveContext) ais_context)
{
	VectorPointer normal( new Vector(0.0,0.0,1.0));
	VectorPointer up( new Vector(0.0,1.0,0.0));
	PointPointer base( new Point(0.0,0.0,0.0));

	SketchPlanePointer my_sketch_plane(new SketchPlane(normal, up, base) );

	Point2DPointer point1(new Point2D(0.0,0.0,my_sketch_plane,false,false));  // none of the dof's can vary
	Point2DPointer point2(new Point2D(10.0,0.0,my_sketch_plane,true,false));  // only x dof can vary
	Point2DPointer point3(new Point2D(10.0,10.0,my_sketch_plane,true,true));  // x and y dof's can vary
	Point2DPointer point4(new Point2D(0.0,10.0,my_sketch_plane,false,true));  // only y dof can vary	

	Line2DPointer line1(new Line2D(point1,point2,my_sketch_plane));
	Line2DPointer line2(new Line2D(point2,point3,my_sketch_plane));
	Line2DPointer line3(new Line2D(point3,point4,my_sketch_plane));
	Line2DPointer line4(new Line2D(point4,point1,my_sketch_plane));

	// create the AIS interactive objects using the ark3d OpenCascade binding
	OccLine2D oc_line1(line1,ais_context);
	OccLine2D oc_line2(line2,ais_context);
	OccLine2D oc_line3(line3,ais_context);
	OccLine2D oc_line4(line4,ais_context);

	OccPoint2D oc_point1(point1,ais_context);
	OccPoint2D oc_point2(point2,ais_context);
	OccPoint2D oc_point3(point3,ais_context);
	OccPoint2D oc_point4(point4,ais_context);

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

	// check some of the DOF's before the solve step
	double x1, y1, z1;
	point1->Get3DLocation(x1, y1, z1);
	cout << "Point 1 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point2->Get3DLocation(x1, y1, z1);
	cout << "Point 2 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point3->Get3DLocation(x1, y1, z1);
	cout << "Point 3 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point4->Get3DLocation(x1, y1, z1);
	cout << "Point 4 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	
	// solve the constraint equations
	my_model.SolveConstraints();

	// check some of the DOF's to see what happened during the solve step 
	point1->Get3DLocation(x1, y1, z1);
	cout << "Point 1 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point2->Get3DLocation(x1, y1, z1);
	cout << "Point 2 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point3->Get3DLocation(x1, y1, z1);
	cout << "Point 3 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;
	point4->Get3DLocation(x1, y1, z1);
	cout << "Point 4 (x,y,z): (" << x1 << ", " << y1 << ", " << z1 << ")" << endl;

}
