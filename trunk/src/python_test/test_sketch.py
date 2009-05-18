from primitives import *
from scipy import pi
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
