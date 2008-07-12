#include <QtGui>

#include "QtPoint2D.h"

QtPoint2D::QtPoint2D (QGraphicsItem * parent,double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free) :
QtPrimitiveBase(parent),
Point2D(s,t,sketch_plane,s_free,t_free)
{
	SetProperties(PointPrimitive);
	SetSelectedProperties(SelectedPointPrimitive);
	SetMouseHoverProperties(HoverPointPrimitive);

	setZValue(GetProperties().GetZ());

	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	// Display the newly create ais_object
	Display();
}

QtPoint2D::QtPoint2D (QGraphicsItem * parent, DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane) :
QtPrimitiveBase(parent),
Point2D(s,t,sketch_plane)
{
	SetProperties(PointPrimitive);
	SetSelectedProperties(SelectedPointPrimitive);
	SetMouseHoverProperties(HoverPointPrimitive);

	setZValue(GetProperties().GetZ());

	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	// Display the newly create ais_object
	Display();
}

void QtPoint2D::UpdateDisplay()
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);


	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtPoint2D::boundingRect() const
{
	QRectF rect(QPointF(GetSValue(),-GetTValue()),
 				QPointF(GetSValue(),-GetTValue()));
	
	// increase the size of the box to account for the actual radius of the point that represents the circle
	// @fixme if the user zooms out or if the model has a large length scale, the bounding box wont contain the whole point
	rect.adjust(-GetBoundingRectPad(),-GetBoundingRectPad(),GetBoundingRectPad(),GetBoundingRectPad());
	
	return rect;
}

void QtPoint2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	DisplayProperties current_properties;

	if(option->state & QStyle::State_MouseOver)
	{
		current_properties = GetMouseHoverProperties();
	} else if (option->state & QStyle::State_Selected) {
		current_properties = GetSelectedProperties();
	} else {
		current_properties = GetProperties();
	}

	painter->setPen(current_properties.GetPen(option->levelOfDetail));
	painter->setBrush(current_properties.GetBrush());	

	QPainterPath point_path;
	PaintPointAndSelectionPath(painter, option, GetSValue(), -GetTValue(),point_path);
	current_shape_ = point_path;
}

