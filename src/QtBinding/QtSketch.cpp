#include "QtSketch.h"

QtSketch::QtSketch(QGraphicsScene *scene, VectorPointer normal, VectorPointer up, PointPointer base, bool grid_snap):
Sketch(normal,up,base),
grid_snap_(grid_snap),
scene_(scene)
{

}

QtPoint2DPointer QtSketch::AddPoint2D ( double s, double t, bool s_free, bool t_free)
{
	QtPoint2DPointer new_point(new QtPoint2D(0,s,t,GetSketchPlane(),s_free,t_free));
	AddPrimitive(new_point);
	return new_point;
}


QtArc2DPointer QtSketch::AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	QtArc2DPointer new_arc(new QtArc2D(0,s_center, t_center, theta_1, theta_2, radius, GetSketchPlane(),s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	AddPrimitive(new_arc);

	// now add the end points and the center of the arc as seperate primitives so that they can be selected by the user for constructing lines and other primitives
	// @fixme these points need to be removed fro the sketch if new_arc is ever deleted from the scene otherwise the arc will still drive the points but will not be visible

	Point2DPointer point1 = new_arc->GetPoint1();
	Point2DPointer point2 = new_arc->GetPoint2();
	Point2DPointer center_point = new_arc->GetCenterPoint();

	QtPoint2DPointer qt_point1(new QtPoint2D(0,point1->GetSDOF(), point1->GetTDOF(), GetSketchPlane()));
	QtPoint2DPointer qt_point2(new QtPoint2D(0,point2->GetSDOF(), point2->GetTDOF(), GetSketchPlane()));
	QtPoint2DPointer qt_center_point(new QtPoint2D(0,center_point->GetSDOF(), center_point->GetTDOF(), GetSketchPlane()));

	// need to explicitly make these points dependent on the arc primitive so that if the arc primitive is ever deleted from the scene, these primitives will be deleted also
	qt_point1->AddPrimitive(new_arc);
	qt_point2->AddPrimitive(new_arc);
	qt_center_point->AddPrimitive(new_arc);

	AddPrimitive(qt_point1);
	AddPrimitive(qt_point2);
	AddPrimitive(qt_center_point);

	return new_arc;
}


QtLine2DPointer QtSketch::AddLine2D (const Point2DPointer point1, const Point2DPointer point2)
{
	QtLine2DPointer new_line(new QtLine2D(0,point1, point2, GetSketchPlane()));
	AddPrimitive(new_line);
	return new_line;
}


QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(0,point1,point2,distance));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

// Add a distance constraint using the current distance
QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(0,point1,point2,1.0));
	new_constraint->SetValue(new_constraint->GetActualDistance());
	new_constraint->UpdateDisplay();
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtParallelLine2DPointer QtSketch::AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	QtParallelLine2DPointer new_constraint (new QtParallelLine2D(0,line1, line2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtAngleLine2DPointer QtSketch::AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle)
{
	QtAngleLine2DPointer new_constraint(new QtAngleLine2D(0,line1,line2,angle,interior_angle));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtTangentEdge2DPointer QtSketch::AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	QtTangentEdge2DPointer new_constraint(new QtTangentEdge2D(0,edge1, point_num_1, edge2, point_num_2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

void QtSketch::AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation)
{
	// if this constraint equation is derived from QtPrimitiveBase add it to the scene
	
	// Normally this type of cast is dangerous since it does the increment share_ptr's reference count
	// It is safe to pass this pointer to QGraphicsScene since QGraphicsItem's destructor removes itself from the QGraphicsScene it is a member of before deleting itself. Becuase of this, scene_ will not contain a nonexistant temp_graphics_item
	QGraphicsItem *temp_graphics_item = dynamic_cast<QGraphicsItem*>(new_constraint_equation.get());

	if(temp_graphics_item != 0)
	{
		scene_->addItem(temp_graphics_item);
		//std::cout << "adding constraint equation to the scene" << std::endl;
	}

	// now call the base class version of this method
	Ark3DModel::AddConstraintEquation(new_constraint_equation);
}

void QtSketch::AddPrimitive(const PrimitiveBasePointer &new_primitive)
{
	// if this primitive is derived from QtPrimitiveBase add it to the scene
	
	// Normally this type of cast is dangerous since it does the increment share_ptr's reference count
	// It is safe to pass this pointer to QGraphicsScene since QGraphicsItem's destructor removes itself from the QGraphicsScene it is a member of before deleting itself. Becuase of this, scene_ will not contain a nonexistant temp_graphics_item
	QGraphicsItem *temp_graphics_item = dynamic_cast<QGraphicsItem*>(new_primitive.get());

	if(temp_graphics_item != 0)
	{
		scene_->addItem(temp_graphics_item);
		//std::cout << "adding primitive to the scene" << std::endl;
	}

	// now call the base class version of this method
	Ark3DModel::AddPrimitive(new_primitive);

}




