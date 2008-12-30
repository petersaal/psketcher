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
#include "DisplayProperties.h"

DisplayProperties::DisplayProperties(DefaultPropertiesSet prop_set) 
{
	arrow_head_length_ = 15.0;
	arrow_head_width_ = 12.0;

	switch (prop_set)
	{
		case Primitive:
			// geometry properties
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 1.0;
			pen_color_ = Qt::black;
			pen_width_ = 1.0;
			brush_ = QBrush(Qt::lightGray,Qt::SolidPattern);
	
			break;
	
		case SelectedPrimitive:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 1.0;
			pen_color_ = QColor(180,180,180);
			pen_width_ = 1.0;
			brush_ = QBrush(QColor(70,70,70),Qt::SolidPattern);
	
			break;

		case HoverPrimitive:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 1.0;
			pen_color_ = QColor(180,180,180);
			pen_width_ = 1.0;
			brush_ = QBrush(QColor(70,70,70),Qt::SolidPattern);
	
			break;
	
		case Annotation:
			
			// annotation properties
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 10.0;
			pen_color_ = QColor(166,86,0);
			pen_width_ = 2.0;
			brush_ = QBrush(pen_color_,Qt::SolidPattern);
	
			break;
	
		case SelectedAnnotation:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 10.0;
			pen_color_ = QColor(180,180,180);
			pen_width_ = 2.0;
			brush_ = QBrush(pen_color_,Qt::SolidPattern);
	
			break;

		case HoverAnnotation:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 10.0;
			pen_color_ = QColor(180,180,180);
			pen_width_ = 2.0;
			brush_ = QBrush(pen_color_,Qt::SolidPattern);
	
			break;
	
		case PointPrimitive:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 2.0;
			pen_color_ = Qt::black;
			pen_width_ = 1.0;
			brush_ = QBrush(Qt::lightGray,Qt::SolidPattern);
	
			break;
	
		case SelectedPointPrimitive:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 2.0;
			pen_color_ = Qt::lightGray;
			pen_width_ = 1.0;
			brush_ = QBrush(Qt::black,Qt::SolidPattern);
	
			break;

		case HoverPointPrimitive:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 2.0;
			pen_color_ = Qt::lightGray;
			pen_width_ = 1.0;
			brush_ = QBrush(Qt::black,Qt::SolidPattern);
	
			break;

		default:
			leader_gap_ = 10.0;
			leader_extension_ = 15.0;
			z_ = 1.0;
			pen_color_ = Qt::black;
			pen_width_ = 2.0;
			brush_ = QBrush(Qt::lightGray,Qt::SolidPattern);
	}
}