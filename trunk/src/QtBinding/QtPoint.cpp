#include "QtPoint.h"


QtPoint::QtPoint (QGraphicsItem * parent, double x, double y, double z, bool x_free, bool y_free, bool z_free) :
QtPrimitiveBase(parent),
Point(x,y,z,x_free,y_free,z_free)
{

	// Display the newly create ais_object
	Display();
}

void QtPoint::UpdateDisplay()
{

	QtPrimitiveBase::UpdateDisplay();
}


