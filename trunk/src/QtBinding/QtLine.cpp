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

#include "QtLine.h"

QtLine::QtLine (QGraphicsItem * parent, const PointPointer point1, const PointPointer point2) :
QtPrimitiveBase(parent),
Line(point1,point2)
{

	// Display the newly create ais_object
	Display();
}

void QtLine::UpdateDisplay()
{
	QtPrimitiveBase::UpdateDisplay();
}

QRectF QtLine::boundingRect() const
{ 
	return QRectF(1,1,1,1);
}

void QtLine::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) 
{
	;
}

