#include <QtGui>

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
	double min_s, max_s, min_t, max_t;
	max_s = min_s = GetLine1()->GetS1()->GetValue();
	max_t= min_t = GetLine1()->GetT1()->GetValue();
	
	double current_s;
	double current_t;	

	for(int i = 0; i<3; i++)
	{
		if (i == 0)
		{
			current_s = GetLine1()->GetS2()->GetValue();
			current_t = GetLine1()->GetT2()->GetValue();	
		} else if (i == 1) {
			current_s = GetLine2()->GetS1()->GetValue();
			current_t = GetLine2()->GetT1()->GetValue();	
		} else if (i == 2) {
			current_s = GetLine2()->GetS2()->GetValue();
			current_t = GetLine2()->GetT2()->GetValue();	
		}	
		
		if(max_s < current_s)
			max_s = current_s;
		else if(min_s > current_s)
			min_s = current_s;
	
		if(max_t < current_t)
			max_t = current_t;
		else if(min_t > current_t)
			min_t = current_t;
	}
	
	return QRectF(QPointF(min_s, -min_t),QPointF(max_s,-max_t));
}

void QtAngleLine2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	painter->setPen(QPen(Qt::black, 1.0/option->levelOfDetail));
	painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));

	// first determine the intersection point of the two lines
	double x1 = GetLine1()->GetPoint1()->GetSValue();
	double x2 = GetLine1()->GetPoint2()->GetSValue();
	double x3 = GetLine2()->GetPoint1()->GetSValue();
	double x4 = GetLine2()->GetPoint2()->GetSValue();

	double y1 = GetLine1()->GetPoint1()->GetTValue();
	double y2 = GetLine1()->GetPoint2()->GetTValue();
	double y3 = GetLine2()->GetPoint1()->GetTValue();
	double y4 = GetLine2()->GetPoint2()->GetTValue();

	double denominator = (x1-x2)*(y3-y4)-(x3-x4)*(y1-y2);

	double text_x, text_y;
	double x_center, y_center;
	bool lines_parallel = false;

	if(denominator == 0.0)
	{
		// Lines are parallel
		lines_parallel = true;

		text_x = (x1 + x2 + x3 + x4) / 4.0;
		text_y = (y1 + y2 + y3 + y4) / 4.0;
	} else {
		// lines do intersect
		// finish calculating the intersection point
		double temp1 = x1*y2-y1*x2;
		double temp2 = x3*y4-x4*y3;
		
		x_center = (temp1*(x3-x4)-temp2*(x1-x2))/denominator;
		y_center = (temp1*(y3-y4)-temp2*(y1-y2))/denominator;

		text_x = x_center + GetTextRadius()*cos(GetTextAngle());
		text_y = y_center + GetTextRadius()*sin(GetTextAngle());
	}

	if(!lines_parallel)
	{
		// normal case where lines are not parallel

		double alpha1 = atan2(y2-y1, x2-x1);
		double theta = acos((x1*x2+y1*y2)/(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*sqrt((x3-x4)*(x3-x4)+(y3-y4)*(y3-y4))));
		
		double alpha2 = alpha1 + theta;
		double alpha3 = alpha1 + mmcPI;
		double alpha4 = alpha1 + mmcPI + theta;
		double alpha5 = alpha1 + 2.0*mmcPI;

		double text_theta = GetTextAngle();

		// determine which sector the text is nearest too
		// first increment text_theta so that it is greater then alpha1 and less then alpha5	
		if(text_theta < alpha1)
		{
			while (text_theta < alpha1)
				text_theta += 2.0*mmcPI;
		} else if (text_theta > alpha5) {
			while (text_theta > alpha5)
				text_theta -= 2.0*mmcPI;
		}
		
		double arrow_arc_theta1;
		double arrow_arc_theta2;
		if(IsInteriorAngle())
		{
			// either sector 1 or sector 3 for an interior angle
			double sector_1_delta = fabs((alpha1+alpha2)*0.5-text_theta);
			double sector_3_delta = fabs((alpha3+alpha4)*0.5-text_theta);

			if(sector_1_delta < sector_3_delta)
			{
				arrow_arc_theta1 = alpha1;
				arrow_arc_theta2 = alpha2;
			} else {
				arrow_arc_theta1 = alpha3;
				arrow_arc_theta2 = alpha4;
			}
		} else {
			// either sector 2 or sector 4 for an exterior angle
			double sector_2_delta = fabs((alpha2+alpha3)*0.5-text_theta);
			double sector_4_delta = fabs((alpha4+alpha5)*0.5-text_theta);

			if(sector_2_delta < sector_4_delta)
			{
				arrow_arc_theta1 = alpha2;
				arrow_arc_theta2 = alpha3;
			} else {
				arrow_arc_theta1 = alpha4;
				arrow_arc_theta2 = alpha5;
			}
		}

		// display the arrow arc
		QPainterPath arrow_arc = GetArcArrowPath(x_center, y_center, GetTextRadius(),arrow_arc_theta1,arrow_arc_theta2,15.0/option->levelOfDetail,12.0/option->levelOfDetail);

		painter->drawPath(arrow_arc);
	} else {
		// degenerate case where lines are parallel

	} // if(!lines_parallel)

	
}

