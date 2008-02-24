
normal = ark3d_module.CreateVector(0.0,0.0,1.0)
up = ark3d_module.CreateVector(0.0,1.0,0.0)
base = ark3d_module.CreatePoint(0.0,0.0,0.0)

my_sketch = ark3d_module.Sketch(normal,up,base)

point1 = my_sketch.AddPoint2D(0.0,0.0,False,False)  # none of the dof's can vary
point2 = my_sketch.AddPoint2D(10.0,0.0,True,False)  # only x dof can vary
point3 = my_sketch.AddPoint2D(10.0,10.0,True,True)  # x and y dof's can vary

print("point 1 = (", point1.GetSDOF().GetValue(), ", ", point1.GetTDOF().GetValue(), ")")
print("point 2 = (", point2.GetSDOF().GetValue(), ", ", point2.GetTDOF().GetValue(), ")")
print("point 3 = (", point3.GetSDOF().GetValue(), ", ", point3.GetTDOF().GetValue(), ")")

pi = 3.1415927
arc1 =  my_sketch.AddArc2D(1.5,6.0,(pi/2.0)*.8,(pi)*1.2,2.0,True,True,True,True,False)
line1 = my_sketch.AddLine2D(point1,point2)
line2 = my_sketch.AddLine2D(point2,point3)
line3 = my_sketch.AddLine2D(point3,arc1.GetPoint1())
line4 = my_sketch.AddLine2D(arc1.GetPoint2(),point1)

# These 5 constraints will fully constrain the four free DOF's defined about
constraint1 = my_sketch.AddDistancePoint2D(point1,point2,6.0)
constraint2 = my_sketch.AddDistancePoint2D(point2,point3,12.0)
constraint3 = my_sketch.AddParallelLine2D(line1,line3)
constraint4 = my_sketch.AddParallelLine2D(line2,line4)
constraint5 = my_sketch.AddAngleLine2D(line1,line2,pi/2.0)
constraint6 = my_sketch.AddTangentEdge2D(line3,ark3d_module.Point2,arc1,ark3d_module.Point1)
constraint7 = my_sketch.AddTangentEdge2D(line4,ark3d_module.Point1,arc1,ark3d_module.Point2)

my_sketch.SolveConstraints()

print("point 1 = (", point1.GetSDOF().GetValue(), ", ", point1.GetTDOF().GetValue(), ")")
print("point 2 = (", point2.GetSDOF().GetValue(), ", ", point2.GetTDOF().GetValue(), ")")
print("point 3 = (", point3.GetSDOF().GetValue(), ", ", point3.GetTDOF().GetValue(), ")")


