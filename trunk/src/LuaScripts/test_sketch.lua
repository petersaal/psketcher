
	normal = ark3d.CreateVector(0.0,0.0,1.0)
	up = ark3d.CreateVector(0.0,1.0,0.0)
	base = ark3d.CreatePoint(0.0,0.0,0.0)

	my_sketch_plane = ark3d.CreateSketchPlane(normal, up, base)

	point1 = ark3d.CreatePoint2D(0.0,0.0,my_sketch_plane,false,false)  -- none of the dof's can vary
	point2 = ark3d.CreatePoint2D(10.0,0.0,my_sketch_plane,true,false)  -- only x dof can vary
	point3 = ark3d.CreatePoint2D(10.0,10.0,my_sketch_plane,true,true)  -- x and y dof's can vary

	print("point 1 = (", point1:GetSDOF():GetValue(), ", ", point1:GetTDOF():GetValue(), ")")
	print("point 2 = (", point2:GetSDOF():GetValue(), ", ", point2:GetTDOF():GetValue(), ")")
	print("point 3 = (", point3:GetSDOF():GetValue(), ", ", point3:GetTDOF():GetValue(), ")")

	pi = 3.1415927
	arc1 =  ark3d.CreateArc2D(1.5,6.0,(pi/2.0)*.8,(pi)*1.2,2.0,my_sketch_plane,true,true,true,true,false)

	line1 = ark3d.CreateLine2D(point1,point2,my_sketch_plane)
	line2 = ark3d.CreateLine2D(point2,point3,my_sketch_plane)
	line3 = ark3d.CreateLine2D(point3,arc1:GetPoint1(),my_sketch_plane)
	line4 = ark3d.CreateLine2D(arc1:GetPoint2(),point1,my_sketch_plane)

	-- These 5 constraints will fully constrain the four free DOF's defined about
	constraint1 = ark3d.CreateDistancePoint2D(point1,point2,6.0)
	constraint2 = ark3d.CreateDistancePoint2D(point2,point3,12.0)
	constraint3 = ark3d.CreateParallelLine2D(line1,line3)
	constraint4 = ark3d.CreateParallelLine2D(line2,line4)
	constraint5 = ark3d.CreateAngleLine2D(line1,line2,pi/2.0)
	constraint6 = ark3d.CreateTangentEdge2D(line3,ark3d.Point2,arc1,ark3d.Point1)
	constraint7 = ark3d.CreateTangentEdge2D(line4,ark3d.Point1,arc1,ark3d.Point2)

	-- Add the primitives to the 3d model object
	ark3d_model_ = ark3d.Ark3DModel()

	ark3d_model_:AddPrimitive(point1)
	ark3d_model_:AddPrimitive(point2)
	ark3d_model_:AddPrimitive(point3)
	ark3d_model_:AddPrimitive(line1)
	ark3d_model_:AddPrimitive(line2)
	ark3d_model_:AddPrimitive(line3)
	ark3d_model_:AddPrimitive(line4)
	ark3d_model_:AddPrimitive(arc1)

	-- Add the constraints to the 3d model object
	ark3d_model_:AddConstraintEquation(constraint1)
	ark3d_model_:AddConstraintEquation(constraint2)
	ark3d_model_:AddConstraintEquation(constraint3)
	ark3d_model_:AddConstraintEquation(constraint4)
	ark3d_model_:AddConstraintEquation(constraint5)
	ark3d_model_:AddConstraintEquation(constraint6)
	ark3d_model_:AddConstraintEquation(constraint7)

	ark3d_model_:SolveConstraints()

	print("point 1 = (", point1:GetSDOF():GetValue(), ", ", point1:GetTDOF():GetValue(), ")")
	print("point 2 = (", point2:GetSDOF():GetValue(), ", ", point2:GetTDOF():GetValue(), ")")
	print("point 3 = (", point3:GetSDOF():GetValue(), ", ", point3:GetTDOF():GetValue(), ")")

io.flush()
