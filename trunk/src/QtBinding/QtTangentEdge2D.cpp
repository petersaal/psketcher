/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

#include <QtGui>

#include "QtTangentEdge2D.h"

#include "QtLine2D.h"
#include "QtArc2D.h"

QtTangentEdge2D::QtTangentEdge2D (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model):
QtPrimitiveBase(parent),
TangentEdge2D(id,psketcher_model)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}

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
	Display();

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtTangentEdge2D::boundingRect() const
{ 
	Edge2DBasePointer edge1 = GetEdge1();
	EdgePointNumber point_num_1 = GetPointNum1();

	Point2DPointer point;

	if(point_num_1 == Point1)
	{	// use point1 of edge1
		point = edge1->GetPoint1();
	} else {
		// use point2 of edge1
		point = edge1->GetPoint2();
	}

	QRectF rect(QPointF(point->GetSValue(),-point->GetTValue()),
 				QPointF(point->GetSValue(),-point->GetTValue()));
	
	// increase the size of the box to account for the actual size of the tangent constraint
	// @fixme if the user zooms out or if the model has a large length scale, the bounding box won't contain the whole tangent constraint
	rect.adjust(-GetBoundingRectPad(),-GetBoundingRectPad(),GetBoundingRectPad(),GetBoundingRectPad());
	
	return rect;
}

void QtTangentEdge2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */) 
{
	DisplayProperties current_properties;

	if(option->state & QStyle::State_MouseOver && IsSelectable())
	{
		current_properties = GetMouseHoverProperties();
	} else if (option->state & QStyle::State_Selected) {
		current_properties = GetSelectedProperties();
	} else {
		current_properties = GetProperties();
	}

	double arrow_head_length = current_properties.GetArrowHeadLength()/option->levelOfDetail;
	double arrow_head_width = current_properties.GetArrowHeadWidth()/option->levelOfDetail;

	painter->setPen(current_properties.GetPen(option->levelOfDetail));
	painter->setBrush(current_properties.GetBrush());	
	
	Edge2DBasePointer edge1 = GetEdge1();
	Edge2DBasePointer edge2 = GetEdge2();
	EdgePointNumber point_num_1 = GetPointNum1();
	EdgePointNumber point_num_2 = GetPointNum2();

	Point2DPointer point1;

	double tangent_1_s;
	double tangent_1_t;

	double tangent_2_s;
	double tangent_2_t;

	if(point_num_1 == Point1)
	{	// use point1 of edge1
		point1 = edge1->GetPoint1();
		edge1->GetTangent1(tangent_1_s,tangent_1_t);
	} else {
		// use point2 of edge1
		point1 = edge1->GetPoint2();
		edge1->GetTangent2(tangent_1_s,tangent_1_t);
	}

	// define the endpoints of the arrow
	double arrow_s1 = point1->GetSValue() + tangent_1_s*2.0*arrow_head_length;
	double arrow_t1 = point1->GetTValue() + tangent_1_t*2.0*arrow_head_length;

	double arrow_s2 = point1->GetSValue() - tangent_1_s*2.0*arrow_head_length;
	double arrow_t2 = point1->GetTValue() - tangent_1_t*2.0*arrow_head_length;

	// draw the arrow
	QPainterPath arrow_selection_path;

	QPolygonF arrow = GetArrowPolygonAndSelectionPath(arrow_s1,-arrow_t1,arrow_s2,-arrow_t2,arrow_head_length,arrow_head_width,arrow_selection_path,option->levelOfDetail);
	current_shape_ = arrow_selection_path;

	painter->drawPolygon(arrow);
}
