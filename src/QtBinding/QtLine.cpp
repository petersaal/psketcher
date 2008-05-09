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
