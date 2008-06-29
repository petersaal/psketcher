#include "QtPoint.h"


QtPoint::QtPoint (QGraphicsItem * parent, double x, double y, double z, bool x_free, bool y_free, bool z_free) :
QtPrimitiveBase(parent),
Point(x,y,z,x_free,y_free,z_free)
{
	SetProperties(PointPrimitive);
	SetSelectedProperties(SelectedPointPrimitive);
	SetMouseHoverProperties(HoverPointPrimitive);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}

void QtPoint::UpdateDisplay()
{

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtPoint::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtPoint::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}


