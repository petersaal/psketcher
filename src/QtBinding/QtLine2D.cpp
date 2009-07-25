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

#include "QtLine2D.h"

QtLine2D::QtLine2D (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model):
QtPrimitiveBase(parent),
Line2D(id,psketcher_model)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// Display the newly create ais_object
	Display();
}

QtLine2D::QtLine2D (QGraphicsItem * parent, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
QtPrimitiveBase(parent),
Line2D(point1, point2, sketch_plane)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// Display the newly create ais_object
	Display();
}

void QtLine2D::UpdateDisplay()
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);


	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtLine2D::boundingRect() const
{
	double min_x = qMin(GetS1()->GetValue(),GetS2()->GetValue());
	double max_x = qMax(GetS1()->GetValue(),GetS2()->GetValue());
	double min_y = qMin(GetT1()->GetValue(),GetT2()->GetValue());
	double max_y = qMax(GetT1()->GetValue(),GetT2()->GetValue());

	return QRectF(QPointF(min_x-GetBoundingRectPad(),-(min_y-GetBoundingRectPad())),QPointF(max_x+GetBoundingRectPad(),-(max_y+GetBoundingRectPad())));
}

void QtLine2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /* widget */)
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

	// draw the line
	QPainterPath line_path;
	painter->drawLine(GetLineAndSelectionPath(GetS1()->GetValue(),-GetT1()->GetValue(),GetS2()->GetValue(),-GetT2()->GetValue(),line_path, option->levelOfDetail));

	// update the shape that will be used for mouse selection
	current_shape_ = line_path;
}
