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
	// paint the actual arc
	painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));

	double radius = GetRadius()->GetValue();
	QRectF rect(QPointF(GetSCenter()->GetValue()-radius,-GetTCenter()->GetValue()-radius),
 				QPointF(GetSCenter()->GetValue()+radius,-GetTCenter()->GetValue()+radius));

	double angle1 = GetTheta1()->GetValue()*((180.0)/(mmcPI));
	double angle2 = GetTheta2()->GetValue()*((180.0)/(mmcPI));

	painter->drawArc(rect,angle1*16.0,(angle2-angle1)*16.0);

	// now paint the end points and the center point
	painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));
	painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));

	// end 2
	PaintPoint(painter, option,s_center_->GetValue()+radius_->GetValue()*cos(theta_1_->GetValue()),
							   -(t_center_->GetValue()+radius_->GetValue()*sin(theta_1_->GetValue())));
	// end 1
	PaintPoint(painter, option,s_center_->GetValue()+radius_->GetValue()*cos(theta_2_->GetValue()),
							   -( t_center_->GetValue()+radius_->GetValue()*sin(theta_2_->GetValue())));

	// center point
	PaintPoint(painter, option,s_center_->GetValue(),-t_center_->GetValue());

	// create the radius dimension if necessary
	// Only display the radius if it is not a free parameter
	// If it is a free parameter, it is not really a constraint and should not be displayed as such
	if( ! radius_->IsFree())
	{
		painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));
		painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));

		QPolygonF radius_arrow = GetArrowPolygon(s_center_->GetValue(),-t_center_->GetValue(),s_center_->GetValue()+radius_->GetValue()*cos(0.5*(theta_1_->GetValue()+theta_2_->GetValue())),
							   -(t_center_->GetValue()+radius_->GetValue()*sin(0.5*(theta_1_->GetValue()+theta_2_->GetValue()))), 15.0/option->levelOfDetail,12.0/option->levelOfDetail);
		painter->drawPolygon(radius_arrow);
	}
}
