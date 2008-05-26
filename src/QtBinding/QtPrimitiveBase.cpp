#include <QtGui>

#include "QtPrimitiveBase.h"

QtPrimitiveBase::QtPrimitiveBase(QGraphicsItem * parent ) : 
QGraphicsItem(parent)
{

}

void QtPrimitiveBase::Display()
{

}

void QtPrimitiveBase::UpdateDisplay()
{
	update();
}

bool QtPrimitiveBase::IsSelected()
{
	return isSelected();
}

void QtPrimitiveBase::SetSelectable(bool selectable)
{
	return setFlag(QGraphicsItem::ItemIsSelectable, selectable);
}

// Erase current ais_object from the ais_context
void QtPrimitiveBase::Erase()
{

}

void QtPrimitiveBase::PaintPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y)
{
	double radius = 3.0/option->levelOfDetail;
	QRectF rect(QPointF(x-radius,y-radius),
 				QPointF(x+radius,y+radius));
	painter->drawEllipse(rect);
}

QPolygonF QtPrimitiveBase::GetArrowPolygon(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, bool double_arrow) const
{
	mmcMatrix start_point(2,1);
	start_point(0,0) = x1;
	start_point(1,0) = y1;

	mmcMatrix end_point(2,1);
	end_point(0,0) = x2;
	end_point(1,0) = y2;
	
	mmcMatrix line_vector = (end_point - start_point).GetNormalized();
	
	mmcMatrix normal_vector(2,1);
	normal_vector(0,0) = line_vector(1,0);
	normal_vector(1,0) = -line_vector(0,0);

	mmcMatrix arrow_base = end_point - arrow_head_length*line_vector;
	mmcMatrix corner_1 = arrow_base + 0.5*arrow_head_width*normal_vector;
	mmcMatrix corner_2 = arrow_base - 0.5*arrow_head_width*normal_vector;

	QPolygonF polygon;

	if(!double_arrow)
	{
		// single ended arrow

		polygon << QPointF(x1,y1) 
				<< QPointF(arrow_base(0,0),arrow_base(1,0)) 
				<< QPointF(corner_1(0,0),corner_1(1,0))
				<< QPointF(end_point(0,0),end_point(1,0))
				<< QPointF(corner_2(0,0),corner_2(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0))
				<< QPointF(x1,y1);
	} else {
		// double ended arrow

		mmcMatrix start_arrow_base = start_point + arrow_head_length*line_vector;
		mmcMatrix start_corner_1 = start_arrow_base + 0.5*arrow_head_width*normal_vector;
		mmcMatrix start_corner_2 = start_arrow_base - 0.5*arrow_head_width*normal_vector;

		polygon << QPointF(start_arrow_base(0,0),start_arrow_base(1,0)) 
				<< QPointF(start_corner_1(0,0),start_corner_1(1,0))
				<< QPointF(start_point(0,0),start_point(1,0))
				<< QPointF(start_corner_2(0,0),start_corner_2(1,0))
				<< QPointF(start_arrow_base(0,0),start_arrow_base(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0)) 
				<< QPointF(corner_1(0,0),corner_1(1,0))
				<< QPointF(end_point(0,0),end_point(1,0))
				<< QPointF(corner_2(0,0),corner_2(1,0))
				<< QPointF(arrow_base(0,0),arrow_base(1,0))
				<< QPointF(start_arrow_base(0,0),start_arrow_base(1,0));
	}

	return polygon;
}