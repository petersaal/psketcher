#include <QtGui>

#include "QtArc2D.h"

QtArc2D::QtArc2D (QGraphicsItem * parent, double s_center, double t_center, double theta_1, double theta_2, double radius, 
					SketchPlanePointer sketch_plane, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, 
					bool radius_free):
QtPrimitiveBase(parent),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane, s_center_free, t_center_free, theta_1_free, theta_2_free,radius_free)
{
	SetProperties(Primitive);
	SetSelectedProperties(SelectedPrimitive);
	SetMouseHoverProperties(HoverPrimitive);

	setZValue(GetProperties().GetZ());

	radius_widget_ = 0;

	// Display the newly create ais_object
	Display();
}

QtArc2D::QtArc2D (QGraphicsItem * parent,DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane):
QtPrimitiveBase(parent),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane)
{
	SetProperties(Primitive);
	SetSelectedProperties(SelectedPrimitive);
	SetMouseHoverProperties(HoverPrimitive);

	setZValue(GetProperties().GetZ());

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
	double radius = qMax(GetRadius()->GetValue(),GetTextRadius()) + GetBoundingRectPad();
	QRectF rect(QPointF(GetSCenter()->GetValue()-radius,-GetTCenter()->GetValue()-radius),
 				QPointF(GetSCenter()->GetValue()+radius,-GetTCenter()->GetValue()+radius));
	return rect;
}

void QtArc2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	DisplayProperties current_properties;

	// @fixme the way radius_properties and point_properties are defined in the following if statement block will prevent the user from changing the display properties of the radius dimension or the points at run time since the DisplayProperties constructor is used to set these properties
	DisplayProperties radius_properties; 
	DisplayProperties point_properties;

	if(option->state & QStyle::State_MouseOver)
	{
		current_properties = GetMouseHoverProperties();
		radius_properties = DisplayProperties(HoverAnnotation);
		point_properties = DisplayProperties(HoverPointPrimitive);
	} else if (option->state & QStyle::State_Selected) {
		current_properties = GetSelectedProperties();
		radius_properties = DisplayProperties(SelectedAnnotation);
		point_properties = DisplayProperties(SelectedPointPrimitive);
	} else {
		current_properties = GetProperties();
		radius_properties = DisplayProperties(Annotation);
		point_properties = DisplayProperties(PointPrimitive);
	}
	
	double leader_gap = current_properties.GetLeaderGap()/option->levelOfDetail;
	double leader_extension = current_properties.GetLeaderExtension()/option->levelOfDetail;

	double leader_extension_angle = ((leader_extension/GetRadius()->GetValue())*(180.0/mmcPI))/option->levelOfDetail;
	double leader_gap_angle = ((leader_gap/GetRadius()->GetValue())*(180.0/mmcPI))/option->levelOfDetail;

	double radius = GetRadius()->GetValue();
	QRectF rect(QPointF(GetSCenter()->GetValue()-radius,-GetTCenter()->GetValue()-radius),
 				QPointF(GetSCenter()->GetValue()+radius,-GetTCenter()->GetValue()+radius));

	double angle1 = GetTheta1()->GetValue()*((180.0)/(mmcPI));
	double angle2 = GetTheta2()->GetValue()*((180.0)/(mmcPI));
	double text_angle = GetTextAngle()*((180.0)/(mmcPI));

	// create the radius dimension if necessary
	// Only display the radius if it is not a free parameter
	// If it is a free parameter, it is not really a constraint and should not be displayed as such
	if( ! radius_->IsFree())
	{
		painter->setPen(radius_properties.GetPen(option->levelOfDetail));
		painter->setBrush(radius_properties.GetBrush());

		QPolygonF radius_arrow = GetArrowPolygon(s_center_->GetValue(),-t_center_->GetValue(),s_center_->GetValue()+radius_->GetValue()*cos(GetTextAngle()),
							   -(t_center_->GetValue()+radius_->GetValue()*sin(GetTextAngle())), 15.0/option->levelOfDetail,12.0/option->levelOfDetail);
		painter->drawPolygon(radius_arrow);

		// extend arc to radius arrow if radius arrow is outside the arc
		if(text_angle < angle1)
		{
			painter->drawArc(rect,(text_angle-leader_extension_angle)*16.0, ((angle1-leader_gap_angle)-(text_angle-leader_extension_angle))*16.0);
		} else if(text_angle > angle2) {
			painter->drawArc(rect,(angle2+leader_gap_angle)*16.0,((text_angle+leader_extension_angle)-(angle2+leader_gap_angle))*16.0);
		}

		// draw a line from the arc center point to the text location in case the text is outside of the arc
		painter->drawLine(QPointF(s_center_->GetValue(),-t_center_->GetValue()),
					      QPointF(s_center_->GetValue()+GetTextRadius()*cos(GetTextAngle()),-(t_center_->GetValue()+GetTextRadius()*sin(GetTextAngle()))));

		// create the line edit widget graphics item
		if(radius_widget_ == 0)
		{
			// @fixme need to make sure the following dyname_cast won't create a pointer that is need used even if this shared_ptr class is freed from memory
			radius_widget_ = new QtArc2DWidget(shared_from_this(),dynamic_cast<QGraphicsItem*>(const_cast<QtArc2D*>(this)));
		}
		radius_widget_->UpdateGeometry(option->levelOfDetail);
	}

	painter->setPen(current_properties.GetPen(option->levelOfDetail));
	painter->setBrush(current_properties.GetBrush());

	// paint the actual arc
	QPainterPath arc_selection_path;
	painter->drawPath(GetArcAndSelectionPath(GetSCenter()->GetValue(), -GetTCenter()->GetValue(), radius, angle1*(mmcPI/180.0), angle2*(mmcPI/180.0), arc_selection_path, option->levelOfDetail));
	current_shape_ = arc_selection_path;

	// now paint the end points and the center point
	painter->setPen(point_properties.GetPen(option->levelOfDetail));
	painter->setBrush(point_properties.GetBrush());

	// end 2
	PaintPoint(painter, option,s_center_->GetValue()+radius_->GetValue()*cos(theta_1_->GetValue()),
							   -(t_center_->GetValue()+radius_->GetValue()*sin(theta_1_->GetValue())));
	// end 1
	PaintPoint(painter, option,s_center_->GetValue()+radius_->GetValue()*cos(theta_2_->GetValue()),
							   -( t_center_->GetValue()+radius_->GetValue()*sin(theta_2_->GetValue())));

	// center point
	PaintPoint(painter, option,s_center_->GetValue(),-t_center_->GetValue());
}







QtArc2DWidget::QtArc2DWidget(QtArc2DPointer arc_primitive, QGraphicsItem *parent) :
arc_primitive_(arc_primitive), QGraphicsProxyWidget(parent)
{
	//setFlags(ItemIgnoresTransformations);

	// create widget
	radius_line_edit_ = new QLineEdit;
	radius_line_edit_->setStyleSheet(LineEditStyleSheet);
	radius_line_edit_->setValidator(new QDoubleValidator(this));
	radius_line_edit_->setAlignment(Qt::AlignCenter);
	radius_line_edit_->setText(QString("%1").arg(arc_primitive_->GetRadiusValue()));
	textChanged();
	//radius_line_edit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	radius_line_edit_->resize(radius_line_edit_->minimumSizeHint());
	connect(radius_line_edit_, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));
	connect(radius_line_edit_, SIGNAL(returnPressed()), this, SLOT(applyChanges()));

	// package widget
	setWidget(radius_line_edit_);
}



// apply the changes if valid values have been entered
void QtArc2DWidget::applyChanges()
{
	if(radius_line_edit_->hasAcceptableInput())
	{
		arc_primitive_->SetRadiusValue(radius_line_edit_->text().toDouble());
		clearFocus();
		emit modelChanged();
	}
}


void QtArc2DWidget::textChanged()
{
	bool acceptable_input;

	acceptable_input = radius_line_edit_->hasAcceptableInput();

	// resize the dialog to automaticall fit all of the text displayed
	QFontMetrics fm(font());
	radius_line_edit_->setFixedWidth(fm.width(radius_line_edit_->text() + "  "));
}

bool QtArc2DWidget::event(QEvent *event)
{
	if(event->type() == QEvent::FocusOut)
	{
		radius_line_edit_->setText(QString("%1").arg(arc_primitive_->GetRadiusValue()));
		textChanged();
	}
	
	return QGraphicsProxyWidget::event(event);
}

void QtArc2DWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{

	QGraphicsProxyWidget::paint(painter, option,widget);	
}

void QtArc2DWidget::UpdateGeometry(double scale)
{
	double text_s;
	double text_t;

	text_s = arc_primitive_->GetSCenterValue() + arc_primitive_->GetTextRadius()*cos(arc_primitive_->GetTextAngle());
	text_t = arc_primitive_->GetTCenterValue() + arc_primitive_->GetTextRadius()*sin(arc_primitive_->GetTextAngle());

	QTransform transform;
	transform.translate(text_s,-text_t);
	transform.scale(1.0/scale, 1.0/scale);
	
	transform.translate(-radius_line_edit_->width()*0.5,-radius_line_edit_->height()*0.5);

	setTransform(transform);
}


