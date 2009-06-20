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

#include "QtHoriVertLine2D.h"

QtHoriVertLine2D::QtHoriVertLine2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model):
QtPrimitiveBase(parent),
HoriVertLine2D(id,ark3d_model)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}

QtHoriVertLine2D::QtHoriVertLine2D(QGraphicsItem * parent, const Line2DPointer line, bool vertical_constraint):
QtPrimitiveBase(parent),
HoriVertLine2D(line,vertical_constraint)
{
	SetProperties(Annotation);
	SetSelectedProperties(SelectedAnnotation);
	SetMouseHoverProperties(HoverAnnotation);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}


void QtHoriVertLine2D::UpdateDisplay()
{
	Display();

	QtPrimitiveBase::UpdateDisplay();
}


QRectF QtHoriVertLine2D::boundingRect() const
{ 
	double min_x = qMin(GetLine()->GetS1()->GetValue(),GetLine()->GetS2()->GetValue());
	double max_x = qMax(GetLine()->GetS1()->GetValue(),GetLine()->GetS2()->GetValue());
	double min_y = qMin(GetLine()->GetT1()->GetValue(),GetLine()->GetT2()->GetValue());
	double max_y = qMax(GetLine()->GetT1()->GetValue(),GetLine()->GetT2()->GetValue());

	return QRectF(QPointF(min_x-GetBoundingRectPad(),-(min_y-GetBoundingRectPad())),QPointF(max_x+GetBoundingRectPad(),-(max_y+GetBoundingRectPad())));
}


void QtHoriVertLine2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
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

	QString marker_text;
	if(IsVerticalConstraint())
		marker_text = "V";
	else
		marker_text = "H";

	// calculate the text position for each line
	mmcMatrix line_point1 = GetLine()->GetPoint1()->GetmmcMatrix();
	mmcMatrix line_point2 = GetLine()->GetPoint2()->GetmmcMatrix();

	mmcMatrix line_text_position = (line_point2 - line_point1)*GetMarkerPosition()+line_point1;

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
	text_path1 = text_transform.translate(line_text_position(0,0),-line_text_position(1,0)).map(text_path1);

	painter->drawPath(text_path1);

	current_shape_ = text_path1;
}
