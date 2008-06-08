#include "QtAngleLine2D.h"


QtAngleLine2D::QtAngleLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle):
QtPrimitiveBase(parent),
AngleLine2D(line1,line2,angle,interior_angle)
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

QRectF QtAngleLine2D::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtAngleLine2D::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}

