#include <QtGui>

#include "QtLine2D.h"


QtLine2D::QtLine2D (QGraphicsItem * parent, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
QtPrimitiveBase(parent),
Line2D(point1, point2, sketch_plane)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// Display the newly create ais_object
	Display();
}

void QtLine2D::UpdateDisplay()
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);


	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtLine2D::boundingRect() const
{
	return QRectF(QPointF(GetS1()->GetValue(),-GetT1()->GetValue()),QPointF(GetS2()->GetValue(),-GetT2()->GetValue()));
}

void QtLine2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));

	QLineF line(QPointF(GetS1()->GetValue(),-GetT1()->GetValue()),QPointF(GetS2()->GetValue(),-GetT2()->GetValue()));
	painter->drawLine(line);
}
