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

#ifndef DisplayPropertiesH
#define DisplayPropertiesH

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QString>

const QString LineEditStyleSheet("QLineEdit { border-width: 2px; border-style: solid; border-color: rgb(166,86,0);}");

enum DefaultPropertiesSet {Primitive, SelectedPrimitive, HoverPrimitive, Annotation, SelectedAnnotation, HoverAnnotation, PointPrimitive, SelectedPointPrimitive, HoverPointPrimitive};

class DisplayProperties
{
	public:
		DisplayProperties(DefaultPropertiesSet prop_set = Primitive);

		// annotation properties
		double GetLeaderGap() 			{return leader_gap_;}
		double GetLeaderExtension() 	{return leader_extension_;}
		double GetArrowHeadLength()		{return arrow_head_length_;}
		double GetArrowHeadWidth()		{return arrow_head_width_;}
		double GetZ() 					{return z_;}
		QColor GetPenColor() 			{return pen_color_;}
		double GetPenWidth() 			{return pen_width_;}
		QPen   GetPen(double scale=1.0)	{return QPen(pen_color_, pen_width_/scale);}
		QBrush GetBrush() 				{return brush_;}


	private:
		// annotation properties
		double leader_gap_;
		double leader_extension_;
		double arrow_head_length_;
		double arrow_head_width_;
		double z_;
		QColor pen_color_;
		double pen_width_;
		QBrush brush_;
};




#endif //DisplayPropertiesH
