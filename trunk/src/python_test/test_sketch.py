from primitives import *
from model import *
from scipy import pi
from math import fabs

model = Model()

for i in range(1):
    point1 = Point2D(0.0,0.0,False,False)
    point2 = Point2D(10.0,0.0,True,False)
    point3 = Point2D(10.0,10.0,True,True)

    arc1 = Arc2D(1.5,6.0,(pi/2.0)*0.8,pi*1.2,2.0,True,True,True,True,False)

    line1 = Line2D(point1,point2)
    line2 = Line2D(point2,point3)
    line3 = Line2D(point3,arc1.point1)
    line4 = Line2D(arc1.point2,point1)

    constraint1 = DistancePoint2D(point1,point2,6.0)
    constraint2 = DistancePoint2D(point2,point3,12.0)
    constraint3 = ParallelLine2D(line1,line3)
    constraint4 = ParallelLine2D(line2,line4)
    constraint5 = AngleLine2D(line1,line2,pi*0.5,False)
    constraint6 = TangentEdge2D(line3,arc1)  #point2,point1
    constraint7 = TangentEdge2D(line4,arc1)  #point1,point2

    model.add_primitives(point1,point2,point3,arc1,line1,line2,line3,line4)
    model.add_constraints(constraint1,constraint2,constraint3,constraint4,constraint5,constraint6,constraint7)

print "Model containts %d primitives" % (len(model.primitive_dict))
print "Model containts %d constraints" % (len(model.constraint_dict))
print "Model containts %d dof's" % (len(model.dof_dict))

print "Starting constraint solver..."

# solve the constraint equations
model.solve_constraints_3()

# check that the constraint solver found the correct solution
epsilon = 1.0e-3
print("point2 location test", fabs(point2.x.value-6.0) < epsilon)
print("point3 location test", fabs(point3.x.value-6.0) < epsilon and \
                              fabs(point3.y.value-12.0) < epsilon)
print("arc1 center location test", fabs(arc1.center_point.x.value-2.0) < epsilon and \
                                   fabs(arc1.center_point.y.value-10.0) < epsilon)
print("theta1 and theta2 test", fabs(arc1.theta1.value-pi/2.0) < epsilon and \
                                fabs(arc1.theta2.value-pi) < epsilon)
