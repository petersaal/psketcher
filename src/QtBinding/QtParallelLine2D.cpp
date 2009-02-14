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

#include "QtParallelLine2D.h"

QtParallelLine2D::QtParallelLine2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model):
QtPrimitiveBase(parent),
ParallelLine2D(id,ark3d_model)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}

QtParallelLine2D::QtParallelLine2D(QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2):
QtPrimitiveBase(parent),
ParallelLine2D(line1,line2)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}


void QtParallelLine2D::UpdateDisplay()
{
	Display();

	QtPrimitiveBase::UpdateDisplay();
}


QRectF QtParallelLine2D::boundingRect() const
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
	
	QRectF bounding_rect = QRectF(QPointF((min_s-GetBoundingRectPad()), -(min_t-GetBoundingRectPad())),QPointF(max_s+GetBoundingRectPad(),-(max_t+GetBoundingRectPad())));

	return bounding_rect;
}


void QtParallelLine2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
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

	painter->setPen(current_properties.GetPen(option->levelOfDetail));
	painter->setBrush(current_properties.GetBrush());

	QFont current_font;
	current_font.setStyleHint(QFont::SansSerif);
	current_font.setFamily("Helvetica");
	current_font.setPointSizeF(12.0);

	QString marker_text("//");

	// calculate the text position for each line
	mmcMatrix line1_point1 = GetLine1()->GetPoint1()->GetmmcMatrix();
	mmcMatrix line1_point2 = GetLine1()->GetPoint2()->GetmmcMatrix();

	mmcMatrix line2_point1 = GetLine2()->GetPoint1()->GetmmcMatrix();
	mmcMatrix line2_point2 = GetLine2()->GetPoint2()->GetmmcMatrix();

	mmcMatrix line1_text_position = (line1_point2 - line1_point1)*GetMarkerPosition()+line1_point1;
	mmcMatrix line2_text_position = (line2_point2 - line2_point1)*GetMarkerPosition()+line2_point1;

	// display the actual text as two painter paths	
	QPainterPath text_path1;
	text_path1.addText (0.0, 0.0, current_font, marker_text);
	QTransform text_transform;
	text_path1 = text_transform.scale(1.0/option->levelOfDetail,1.0/option->levelOfDetail).map(text_path1);
	text_transform.reset();
	QRectF bounding_box_1 = text_path1.boundingRect();
	text_transform.reset();
	text_path1 = text_transform.translate(-bounding_box_1.center().x(),-bounding_box_1.center().y()).map(text_path1);
	text_transform.reset();
	text_path1 = text_transform.translate(line1_text_position(0,0),-line1_text_position(1,0)).map(text_path1);

	QPainterPath text_path2; 
	text_path2.addText (0.0, 0.0, current_font, marker_text);
	text_transform.reset();
	text_path2 = text_transform.scale(1.0/option->levelOfDetail,1.0/option->levelOfDetail).map(text_path2);
	text_transform.reset();
	QRectF bounding_box_2 = text_path2.boundingRect();
	text_transform.reset();
	text_path2 = text_transform.translate(-bounding_box_2.center().x(),-bounding_box_2.center().y()).map(text_path2);
	text_transform.reset();
	text_path2 = text_transform.translate(line2_text_position(0,0),-line2_text_position(1,0)).map(text_path2);

	painter->drawPath(text_path1);
	painter->drawPath(text_path2);

	current_shape_ = text_path1;
	current_shape_.addPath(text_path2);
}
