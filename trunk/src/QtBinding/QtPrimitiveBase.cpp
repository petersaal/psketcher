#include <QtGui>

#include "QtPrimitiveBase.h"

QtPrimitiveBase::QtPrimitiveBase(QGraphicsItem * parent ) : 
QGraphicsItem(parent)
{
	// by default use primitive display properties
	SetProperties(Primitive);
	SetSelectedProperties(SelectedPrimitive);
	SetMouseHoverProperties(HoverPrimitive);
}

void QtPrimitiveBase::Display()
{

}

void QtPrimitiveBase::UpdateDisplay()
{
	update();
}

bool QtPrimitiveBase::IsSelected()
{
	return isSelected();
}

void QtPrimitiveBase::SetSelectable(bool selectable)
{
	setFlag(QGraphicsItem::ItemIsSelectable, selectable);
}

// Erase current ais_object from the ais_context
void QtPrimitiveBase::Erase()
{

}

void QtPrimitiveBase::PaintPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y)
{
	double radius = 3.0/option->levelOfDetail;
	QRectF rect(QPointF(x-radius,y-radius),
 				QPointF(x+radius,y+radius));
	painter->drawEllipse(rect);
}

QPolygonF QtPrimitiveBase::GetArrowPolygon(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, bool double_arrow) const
{
	mmcMatrix start_point(2,1);
	start_point(0,0) = x1;
	start_point(1,0) = y1;

	mmcMatrix end_point(2,1);
	end_point(0,0) = x2;
	end_point(1,0) = y2;
	
	mmcMatrix line_vector = (end_point - start_point).GetNormalized();
	
	mmcMatrix normal_vector(2,1);
	normal_vector(0,0) = line_vector(1,0);
	normal_vector(1,0) = -line_vector(0,0);

	mmcMatrix arrow_base = end_point - arrow_head_length*line_vector;
	mmcMatrix corner_1 = arrow_base + 0.5*arrow_head_width*normal_vector;
	mmcMatrix corner_2 = arrow_base - 0.5*arrow_head_width*normal_vector;

	QPolygonF polygon;

	if(!double_arrow)
	{
		// single ended arrow

		polygon << QPointF(x1,y1) 
				<< QPointF(arrow_base(0,0),arrow_base(1,0)) 
				<< QPointF(corner_1(0,0),corner_1(1,0))
				<< QPointF(end_point(0,0),end_point(1,0))
				<< QPointF(corner_2(0,0),corner_2(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0))
				<< QPointF(x1,y1);
	} else {
		// double ended arrow

		mmcMatrix start_arrow_base = start_point + arrow_head_length*line_vector;
		mmcMatrix start_corner_1 = start_arrow_base + 0.5*arrow_head_width*normal_vector;
		mmcMatrix start_corner_2 = start_arrow_base - 0.5*arrow_head_width*normal_vector;

		polygon << QPointF(start_arrow_base(0,0),start_arrow_base(1,0)) 
				<< QPointF(start_corner_1(0,0),start_corner_1(1,0))
				<< QPointF(start_point(0,0),start_point(1,0))
				<< QPointF(start_corner_2(0,0),start_corner_2(1,0))
				<< QPointF(start_arrow_base(0,0),start_arrow_base(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0)) 
				<< QPointF(corner_1(0,0),corner_1(1,0))
				<< QPointF(end_point(0,0),end_point(1,0))
				<< QPointF(corner_2(0,0),corner_2(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0))
				<< QPointF(start_arrow_base(0,0),start_arrow_base(1,0));
	}

	return polygon;
}

QPainterPath QtPrimitiveBase::GetArcArrowPath(double x_center, double y_center, double radius, double theta1, double theta2, double arrow_head_length, double arrow_head_width) const
{
	// theta1 and theta2 are in radians
	
	QPainterPath path;

	// define everyting that will be needed to create the arc arrow
	QRectF rect(QPointF(x_center-radius,y_center-radius),
 				QPointF(x_center+radius,y_center+radius));
	
	double arrow_base_theta1;
	double arrow_base_theta2;

	if(theta2 >= theta1)
	{
		arrow_base_theta1 = theta1 + arrow_head_length/radius;
		arrow_base_theta2 = theta2 - arrow_head_length/radius;
	} else {
		arrow_base_theta1 = theta1 - arrow_head_length/radius;
		arrow_base_theta2 = theta2 + arrow_head_length/radius;
	}

	mmcMatrix normal_1(2,1);
	mmcMatrix normal_2(2,1);

	mmcMatrix base_point_1;
	mmcMatrix base_point_2;
	mmcMatrix tip_point_1(2,1);
	mmcMatrix tip_point_2(2,1);

	mmcMatrix center(2,1);
	center(0,0) = x_center;
	center(1,0) = y_center;

	tip_point_1(0,0) = radius*cos(theta1);
	tip_point_1(1,0) = -radius*sin(theta1);
	tip_point_1 = tip_point_1 + center;

	normal_1(0,0) = cos(arrow_base_theta1);
	normal_1(1,0) = -sin(arrow_base_theta1);

	base_point_1 = radius * normal_1 + center;

	tip_point_2(0,0) = radius*cos(theta2);
	tip_point_2(1,0) = -radius*sin(theta2);
	tip_point_2 = tip_point_2 + center;

	normal_2(0,0) = cos(arrow_base_theta2);
	normal_2(1,0) = -sin(arrow_base_theta2);

	base_point_2 = radius * normal_2 + center;

	mmcMatrix arrow_1_corner_1 = base_point_1 + 0.5*arrow_head_width*normal_1;
	mmcMatrix arrow_1_corner_2 = base_point_1 - 0.5*arrow_head_width*normal_1;

	mmcMatrix arrow_2_corner_1 = base_point_2 + 0.5*arrow_head_width*normal_2;
	mmcMatrix arrow_2_corner_2 = base_point_2 - 0.5*arrow_head_width*normal_2;

	// create the arc arrow
	path.moveTo(base_point_1(0,0),base_point_1(1,0));
	path.lineTo(arrow_1_corner_1(0,0), arrow_1_corner_1(1,0));
	path.lineTo(tip_point_1(0,0), tip_point_1(1,0));
	path.lineTo(arrow_1_corner_2(0,0), arrow_1_corner_2(1,0));
	//path.moveTo(base_point_1(0,0),base_point_1(1,0));
	path.arcTo(rect,arrow_base_theta1*(180.0/mmcPI),(arrow_base_theta2-arrow_base_theta1)*(180.0/mmcPI));
	path.lineTo(arrow_2_corner_1(0,0), arrow_2_corner_1(1,0));
	path.lineTo(tip_point_2(0,0), tip_point_2(1,0));
	path.lineTo(arrow_2_corner_2(0,0), arrow_2_corner_2(1,0));

	path.arcTo(rect,arrow_base_theta2*(180.0/mmcPI),(arrow_base_theta1-arrow_base_theta2)*(180.0/mmcPI));

	return path;
}