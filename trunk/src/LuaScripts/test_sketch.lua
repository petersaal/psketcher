
	normal = ark3d.Vector(0.0,0.0,1.0)
	up = ark3d.Vector(0.0,1.0,0.0)
	base = ark3d.Point(0.0,0.0,0.0)

	my_sketch_plane = ark3d.SketchPlane(normal, up, base)

	point1 = ark3d.Point2D(0.0,0.0,my_sketch_plane,false,false)  -- none of the dof's can vary
	point2 = ark3d.Point2D(10.0,0.0,my_sketch_plane,true,false)  -- only x dof can vary
	point3 = ark3d.Point2D(10.0,10.0,my_sketch_plane,true,true)  -- x and y dof's can vary

	print( point1:GetSDOF():GetValue() )

--[[

	pi = 3.1415927
	arc1 =  ark3d.Arc2D(1.5,6.0,(pi/2.0)*.8,(pi)*1.2,2.0,my_sketch_plane,true,true,true,true,false)

	line1 = ark3d.Line2D(point1,point2,my_sketch_plane)
	line2 = ark3d.Line2D(point2,point3,my_sketch_plane)
	line3 = ark3d.Line2D(point3,arc1:GetPoint1(),my_sketch_plane)
	line4 = ark3d.Line2D(arc1:GetPoint2(),point1,my_sketch_plane)

	// These 5 constraints will fully constrain the four free DOF's defined about
	ConstraintEquationBasePointer constraint1(new OccDistancePoint2D(myContext,point1,point2,6.0));
	ConstraintEquationBasePointer constraint2(new OccDistancePoint2D(myContext,point2,point3,12.0));
	ConstraintEquationBasePointer constraint3(new OccParallelLine2D(myContext,line1,line3));
	ConstraintEquationBasePointer constraint4(new OccParallelLine2D(myContext,line2,line4));
	ConstraintEquationBasePointer constraint5(new OccAngleLine2D(myContext,line1,line2,mmcPI/2.0));

	ConstraintEquationBasePointer constraint6(new TangentEdge2D(line3,Point2,arc1,Point1));
	ConstraintEquationBasePointer constraint7(new TangentEdge2D(line4,Point1,arc1,Point2));
	
	// create an edge loop
	EdgeLoop2DPointer edge_loop1(new EdgeLoop2D());
	edge_loop1->AddEdge(line1);
	edge_loop1->AddEdge(line2);
	edge_loop1->AddEdge(line3);
	edge_loop1->AddEdge(arc1);
	edge_loop1->AddEdge(line4);
	cout << "Is loop valid: " << edge_loop1->IsLoopValid() << endl;

	// Add the primitives to the 3d model object
	ark3d_model_.AddPrimitive(point1);
	ark3d_model_.AddPrimitive(point2);
	ark3d_model_.AddPrimitive(point3);
	ark3d_model_.AddPrimitive(line1);
	ark3d_model_.AddPrimitive(line2);
	ark3d_model_.AddPrimitive(line3);
	ark3d_model_.AddPrimitive(line4);
	ark3d_model_.AddPrimitive(arc1);

	// Add the constraints to the 3d model object
	ark3d_model_.AddConstraintEquation(constraint1);
	ark3d_model_.AddConstraintEquation(constraint2);
	ark3d_model_.AddConstraintEquation(constraint3);
	ark3d_model_.AddConstraintEquation(constraint4);
	ark3d_model_.AddConstraintEquation(constraint5);
	ark3d_model_.AddConstraintEquation(constraint6);
	ark3d_model_.AddConstraintEquation(constraint7);

--]]

io.flush()

