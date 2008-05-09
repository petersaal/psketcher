#include "QtParallelLine2D.h"

QtParallelLine2D::QtParallelLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2):
QtPrimitiveBase(parent),
ParallelLine2D(line1,line2)
{
	// Display the newly create ais_object
	Display();
}


void QtParallelLine2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

