#include "QtLine.h"

QtLine::QtLine (QGraphicsItem * parent, const PointPointer point1, const PointPointer point2) :
QtPrimitiveBase(parent),
Line(point1,point2)
{

	// Display the newly create ais_object
	Display();
}

void QtLine::UpdateDisplay()
{
	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtLine::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtLine::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}

