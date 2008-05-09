#include "QtAngleLine2D.h"


QtAngleLine2D::QtAngleLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2, double angle):
QtPrimitiveBase(parent),
AngleLine2D(line1,line2,angle)
{
	// Display the newly create ais_object
	Display();
}


void QtAngleLine2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

