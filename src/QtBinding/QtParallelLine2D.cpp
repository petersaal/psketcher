#include "QtParallelLine2D.h"

QtParallelLine2D::QtParallelLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2):
QtPrimitiveBase(parent),
ParallelLine2D(line1,line2)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}


void QtParallelLine2D::UpdateDisplay()
{
	Display();

	QtPrimitiveBase::UpdateDisplay();
}


QRectF QtParallelLine2D::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtParallelLine2D::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}
