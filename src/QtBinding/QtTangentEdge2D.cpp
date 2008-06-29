#include "QtTangentEdge2D.h"

#include "QtLine2D.h"
#include "QtArc2D.h"

QtTangentEdge2D::QtTangentEdge2D (QGraphicsItem * parent,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2):
QtPrimitiveBase(parent),
TangentEdge2D(edge1,point_num_1,edge2,point_num_2)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}


void QtTangentEdge2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtTangentEdge2D::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtTangentEdge2D::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}
