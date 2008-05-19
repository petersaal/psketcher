#include <QtGui>

#include "QtArc2D.h"

QtArc2D::QtArc2D (QGraphicsItem * parent, double s_center, double t_center, double theta_1, double theta_2, double radius, 
					SketchPlanePointer sketch_plane, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, 
					bool radius_free):
QtPrimitiveBase(parent),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane, s_center_free, t_center_free, theta_1_free, theta_2_free,radius_free)
{
	// Display the newly create ais_object
	Display();
}

QtArc2D::QtArc2D (QGraphicsItem * parent,DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane):
QtPrimitiveBase(parent),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane)
{
	// Display the newly create ais_object
	Display();
}


void QtArc2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtArc2D::boundingRect() const
{
	double radius = GetRadius()->GetValue();
	QRectF rect(QPointF(GetSCenter()->GetValue()-radius,-GetTCenter()->GetValue()-radius),
 				QPointF(GetSCenter()->GetValue()+radius,-GetTCenter()->GetValue()+radius));
	return rect;
}

void QtArc2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	
	painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));

	double radius = GetRadius()->GetValue();
	QRectF rect(QPointF(GetSCenter()->GetValue()-radius,-GetTCenter()->GetValue()-radius),
 				QPointF(GetSCenter()->GetValue()+radius,-GetTCenter()->GetValue()+radius));

	double angle1 = GetTheta1()->GetValue()*((180.0)/(mmcPI));
	double angle2 = GetTheta2()->GetValue()*((180.0)/(mmcPI));

	painter->drawArc(rect,angle1*16.0,(angle2-angle1)*16.0);
	
}
