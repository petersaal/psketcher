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

#include "QtPoint.h"

QtPoint::QtPoint (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model):
QtPrimitiveBase(parent),
Point(id,psketcher_model)
{
	SetProperties(PointPrimitive);
	SetSelectedProperties(SelectedPointPrimitive);
	SetMouseHoverProperties(HoverPointPrimitive);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}


QtPoint::QtPoint (QGraphicsItem * parent, double x, double y, double z, bool x_free, bool y_free, bool z_free) :
QtPrimitiveBase(parent),
Point(x,y,z,x_free,y_free,z_free)
{
	SetProperties(PointPrimitive);
	SetSelectedProperties(SelectedPointPrimitive);
	SetMouseHoverProperties(HoverPointPrimitive);

	setZValue(GetProperties().GetZ());

	// Display the newly create ais_object
	Display();
}

void QtPoint::UpdateDisplay()
{

	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtPoint::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtPoint::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}


