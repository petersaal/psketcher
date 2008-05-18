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
	return QRectF(1,1,1,1);
}

void QtDistancePoint2D::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}

