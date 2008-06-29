#include <QtGui>

#include "QtAngleLine2D.h"

QtAngleLine2D::QtAngleLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle):
QtPrimitiveBase(parent),
AngleLine2D(line1,line2,angle,interior_angle)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	angle_widget_ = 0;
	
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
	// @fixme need to handle the case where one of the lines has zero length (make sure there are no divide by zeros)

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
	
	double leader_gap = current_properties.GetLeaderGap()/option->levelOfDetail;
	double leader_extension = current_properties.GetLeaderExtension()/option->levelOfDetail;

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
	double arrow_tip_x1;
	double arrow_tip_x2;
	double arrow_tip_y1;
	double arrow_tip_y2;

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
		double line1_theta = atan2(y2-y1, x2-x1);
		double line2_theta = atan2(y4-y3, x4-x3);
		if(line2_theta < line1_theta)
			line2_theta += 2.0*mmcPI;

		double theta;
		bool theta_is_interior;	

		if(line2_theta - line1_theta > mmcPI)
		{
			theta_is_interior = false;
			theta = line2_theta - line1_theta - mmcPI;
		} else {
			theta_is_interior = true;
			theta = line2_theta - line1_theta;
		}

		double alpha1 = line1_theta;

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
		if(IsInteriorAngle() && theta_is_interior || !IsInteriorAngle() && !theta_is_interior)
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

			// theta1 correspnds to line1 
			arrow_tip_x1 = x_center + GetTextRadius()*cos(arrow_arc_theta1);
			arrow_tip_y1 = y_center + GetTextRadius()*sin(arrow_arc_theta1);

			arrow_tip_x2 = x_center + GetTextRadius()*cos(arrow_arc_theta2);
			arrow_tip_y2 = y_center + GetTextRadius()*sin(arrow_arc_theta2);

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

			// theta1 correspnds to line2 
			arrow_tip_x2 = x_center + GetTextRadius()*cos(arrow_arc_theta1);
			arrow_tip_y2 = y_center + GetTextRadius()*sin(arrow_arc_theta1);

			arrow_tip_x1 = x_center + GetTextRadius()*cos(arrow_arc_theta2);
			arrow_tip_y1 = y_center + GetTextRadius()*sin(arrow_arc_theta2);
		}

		// display the arrow arc
		QPainterPath arrow_arc = GetArcArrowPath(x_center, -y_center, GetTextRadius(),arrow_arc_theta1,arrow_arc_theta2,15.0/option->levelOfDetail,12.0/option->levelOfDetail);
		painter->drawPath(arrow_arc);
		
		// draw an additional arc to the text if text is outside of the arrow arc
		QRectF rect(QPointF(x_center-GetTextRadius(),-y_center-GetTextRadius()),
					QPointF(x_center+GetTextRadius(),-y_center+GetTextRadius()));
		if(text_theta < arrow_arc_theta1)
			painter->drawArc(rect,(text_theta)*(180.0/mmcPI)*16.0,(arrow_arc_theta1-text_theta)*(180.0/mmcPI)*16.0);
		else if (text_theta > arrow_arc_theta2) 
			painter->drawArc(rect,(arrow_arc_theta2)*(180.0/mmcPI)*16.0,((text_theta)-(arrow_arc_theta2))*(180.0/mmcPI)*16.0);

		// display the editable text
		// create the line edit widget graphics item
		if(angle_widget_ == 0)
		{
			// @fixme need to make sure the following dyname_cast won't create a pointer that is need used even if this shared_ptr class is freed from memory
			angle_widget_ = new QtAngleLine2DWidget(shared_from_this(),dynamic_cast<QGraphicsItem*>(const_cast<QtAngleLine2D*>(this)));
		}
		angle_widget_->UpdateGeometry(text_x, text_y, option->levelOfDetail);

	} else {
		// the case where the lines are parallel
		// @fixme need to implement QtAngleLine2D::paint method for case when the two lines are parallel		
		
	} // if(!lines_parallel)

	// create leader for line1 if necessary
	double delta1, delta2;
	double line1_length = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	delta1 = sqrt((arrow_tip_x1-x1)*(arrow_tip_x1-x1)+(arrow_tip_y1-y1)*(arrow_tip_y1-y1));
	delta2 = sqrt((arrow_tip_x1-x2)*(arrow_tip_x1-x2)+(arrow_tip_y1-y2)*(arrow_tip_y1-y2));
	if(delta1 > line1_length && delta1 > delta2)
	{
		double direction_x = arrow_tip_x1-x1;
		double direction_y = arrow_tip_y1-y1;
		double direction_length = sqrt(direction_x*direction_x + direction_y*direction_y);
		direction_x = direction_x / direction_length;
		direction_y = direction_y / direction_length;

		painter->drawLine(QPointF(x2+leader_gap*direction_x,-(y2+leader_gap*direction_y)),
						  QPointF(x1+(delta1+leader_extension)*direction_x,-(y1+(delta1+leader_extension)*direction_y)));
	} else if (delta2 > line1_length) {
		double direction_x = arrow_tip_x1-x2;
		double direction_y = arrow_tip_y1-y2;
		double direction_length = sqrt(direction_x*direction_x + direction_y*direction_y);
		direction_x = direction_x / direction_length;
		direction_y = direction_y / direction_length;

		painter->drawLine(QPointF(x1+leader_gap*direction_x,-(y1+leader_gap*direction_y)),
						  QPointF(x2+(delta2+leader_extension)*direction_x,-(y2+(delta2+leader_extension)*direction_y)));
	}

	// create leader for line2 if necessary
	double line2_length = sqrt((x3-x4)*(x3-x4)+(y3-y4)*(y3-y4));
	delta1 = sqrt((arrow_tip_x2-x3)*(arrow_tip_x2-x3)+(arrow_tip_y2-y3)*(arrow_tip_y2-y3));
	delta2 = sqrt((arrow_tip_x2-x4)*(arrow_tip_x2-x4)+(arrow_tip_y2-y4)*(arrow_tip_y2-y4));
	if(delta1 > line2_length && delta1 > delta2)
	{
		double direction_x = arrow_tip_x2-x3;
		double direction_y = arrow_tip_y2-y3;
		double direction_length = sqrt(direction_x*direction_x + direction_y*direction_y);
		direction_x = direction_x / direction_length;
		direction_y = direction_y / direction_length;

		painter->drawLine(QPointF(x4+leader_gap*direction_x,-(y4+leader_gap*direction_y)),
						  QPointF(x3+(delta1+leader_extension)*direction_x,-(y3+(delta1+leader_extension)*direction_y)));
	} else if (delta2 > line2_length) {
		double direction_x = arrow_tip_x2-x4;
		double direction_y = arrow_tip_y2-y4;
		double direction_length = sqrt(direction_x*direction_x + direction_y*direction_y);
		direction_x = direction_x / direction_length;
		direction_y = direction_y / direction_length;

		painter->drawLine(QPointF(x3+leader_gap*direction_x,-(y3+leader_gap*direction_y)),
						  QPointF(x4+(delta2+leader_extension)*direction_x,-(y4+(delta2+leader_extension)*direction_y)));
	}
	
}






QtAngleLine2DWidget::QtAngleLine2DWidget(QtAngleLine2DPointer arc_primitive, QGraphicsItem *parent) :
angle_constraint_primitive_(arc_primitive), QGraphicsProxyWidget(parent)
{
	//setFlags(ItemIgnoresTransformations);

	// create widget
	angle_line_edit_ = new QLineEdit;
	angle_line_edit_->setStyleSheet(LineEditStyleSheet);
	angle_line_edit_->setValidator(new QDoubleValidator(this));
	angle_line_edit_->setAlignment(Qt::AlignCenter);
	angle_line_edit_->setText(QString("%1").arg(angle_constraint_primitive_->GetAngleValue()*(180.0/mmcPI)));
	textChanged();
	//angle_line_edit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	angle_line_edit_->resize(angle_line_edit_->minimumSizeHint());
	connect(angle_line_edit_, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));
	connect(angle_line_edit_, SIGNAL(returnPressed()), this, SLOT(applyChanges()));

	// package widget
	setWidget(angle_line_edit_);
}



// apply the changes if valid values have been entered
void QtAngleLine2DWidget::applyChanges()
{
	if(angle_line_edit_->hasAcceptableInput())
	{
		angle_constraint_primitive_->SetAngleValue(angle_line_edit_->text().toDouble()*(mmcPI/180.0));
		clearFocus();
		emit modelChanged();
	}
}


void QtAngleLine2DWidget::textChanged()
{
	bool acceptable_input;

	acceptable_input = angle_line_edit_->hasAcceptableInput();

	// resize the dialog to automaticall fit all of the text displayed
	QFontMetrics fm(font());
	angle_line_edit_->setFixedWidth(fm.width(angle_line_edit_->text() + "  "));
}

bool QtAngleLine2DWidget::event(QEvent *event)
{
	if(event->type() == QEvent::FocusOut)
	{
		angle_line_edit_->setText(QString("%1").arg(angle_constraint_primitive_->GetAngleValue()*(180.0/mmcPI)));
		textChanged();
	}
	
	return QGraphicsProxyWidget::event(event);
}

void QtAngleLine2DWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
	QGraphicsProxyWidget::paint(painter, option,widget);	
}

void QtAngleLine2DWidget::UpdateGeometry(double text_s, double text_t, double scale)
{
	QTransform transform;
	transform.translate(text_s,-text_t);
	transform.scale(1.0/scale, 1.0/scale);
	
	transform.translate(-angle_line_edit_->width()*0.5,-angle_line_edit_->height()*0.5);

	setTransform(transform);
}

