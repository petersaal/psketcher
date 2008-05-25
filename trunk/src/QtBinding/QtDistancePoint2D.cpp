#include <QtGui>

#include "QtDistancePoint2D.h"

QtDistancePoint2D::QtDistancePoint2D(QGraphicsItem * parent,const Point2DPointer point1, const Point2DPointer point2,double distance):
QtPrimitiveBase(parent),
DistancePoint2D(point1,point2,distance)
{
	// Display the newly create ais_object
	Display();
}


void QtDistancePoint2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtDistancePoint2D::boundingRect() const
{ 
	return QRectF(QPointF(GetPoint1()->GetSValue(),-GetPoint1()->GetTValue()),QPointF(GetPoint2()->GetSValue(),-GetPoint2()->GetTValue()));
}

void QtDistancePoint2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
{
	// some constants that need to eventually be set elsewhere
	// @fixme the following paint related constants need to be defined as class members
	double arrow_head_length = 15.0/option->levelOfDetail;
	double arrow_head_width = 12.0/option->levelOfDetail;
	double line_width = 1.0/option->levelOfDetail;
	double leader_gap = 10.0/option->levelOfDetail;
	double leader_extension = 15.0/option->levelOfDetail;
	// end of constants

	painter->setPen(QPen(Qt::black, line_width));
	painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));

	mmcMatrix point1 = point1_->GetmmcMatrix();
	mmcMatrix point2 = point2_->GetmmcMatrix();
	mmcMatrix tangent = (point2-point1).GetNormalized();

	mmcMatrix normal(2,1);
	normal(0,0) = -tangent(1,0);
	normal(1,0) = tangent(0,0);
	
	mmcMatrix text_location(2,1);
	text_location(0,0) = text_s_;
	text_location(1,0) = text_t_;
	
	double offset = (text_location - point1).DotProduct(normal);
	double offset_sign;
	if (offset >= 1)
		offset_sign = 1.0;
	else
		offset_sign = -1.0;
			

	mmcMatrix arrow_end_1 = point1 + offset*normal;
	mmcMatrix arrow_end_2 = point2 + offset*normal;

	mmcMatrix leader_start_1 = point1 + offset_sign*leader_gap*normal;
	mmcMatrix leader_start_2 = point2 + offset_sign*leader_gap*normal;
	mmcMatrix leader_end_1 = point1 + (offset+offset_sign*leader_extension)*normal;
	mmcMatrix leader_end_2 = point2 + (offset+offset_sign*leader_extension)*normal;

	painter->drawLine(QPointF(leader_start_1(0,0),-leader_start_1(1,0)),QPointF(leader_end_1(0,0),-leader_end_1(1,0)));
	painter->drawLine(QPointF(leader_start_2(0,0),-leader_start_2(1,0)),QPointF(leader_end_2(0,0),-leader_end_2(1,0)));

	QPolygonF arrow = GetArrowPolygon(arrow_end_1(0,0),-arrow_end_1(1,0),arrow_end_2(0,0),-arrow_end_2(1,0),arrow_head_length,arrow_head_width,true);

	painter->drawPolygon(arrow);
}

