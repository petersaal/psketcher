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

#ifndef QtPrimitiveBaseH
#define QtPrimitiveBaseH

#include "DisplayProperties.h"

#include <QGraphicsItem>
#include <QRectF>
#include <QLineF>
#include "../ConstraintSolver/Primitives.h"

// QtPrimitiveBase class
class QtPrimitiveBase : public QGraphicsItem, virtual public PrimitiveBase
{
	public:
		QtPrimitiveBase(QGraphicsItem * parent = 0);

		virtual void Display();
		virtual void Erase();

		DisplayProperties GetProperties()           {return properties_;}
		DisplayProperties GetSelectedProperties()   {return selected_properties_;}
		DisplayProperties GetMouseHoverProperties() {return mouse_hover_properties_;}
	
		void SetProperties(DefaultPropertiesSet prop_set) {properties_ = DisplayProperties(prop_set);}
		void SetSelectedProperties(DefaultPropertiesSet prop_set) {selected_properties_ = DisplayProperties(prop_set);}
		void SetMouseHoverProperties(DefaultPropertiesSet prop_set) {mouse_hover_properties_ = DisplayProperties(prop_set);}

		double GetBoundingRectPad() const {return bounding_rect_pad_;}

		// update coordinates of primitive
		virtual void UpdateDisplay();

		virtual bool IsSelected();
		virtual void SetSelectable(bool selectable_);

		// some utility methods used to paint primitives
		void PaintPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y);
		void PaintPointAndSelectionPath(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y,QPainterPath &selection_path);

		QPolygonF GetArrowPolygon(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, bool double_arrow = false) const;
		QPainterPath GetArcArrowPath(double x_center, double y_center, double radius, double theta1, double theta2, double arrow_head_length, double arrow_head_width) const;

		QLineF GetLineAndSelectionPath(double x1, double y1, double x2, double y2, QPainterPath &selection_path, double scale) const;
		QLineF GetLineAndSelectionPath(mmcMatrix point1, mmcMatrix point2, QPainterPath &selection_path, double scale) const;
		
		QPainterPath GetArcAndSelectionPath(double center_x, double center_y, double radius, double theta1, double theta2, QPainterPath &selection_path, double scale) const;

		QPolygonF GetArrowPolygonAndSelectionPath(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, QPainterPath &selection_path, double scale) const;
		QPainterPath GetArcArrowPathAndSelectionPath(double x_center, double y_center, double radius, double theta1, double theta2, double arrow_head_length, double arrow_head_width, QPainterPath &selection_path, double scale) const;

	protected:
		// member class that stores display properties for the primitives
		DisplayProperties properties_;
		DisplayProperties selected_properties_;
		DisplayProperties mouse_hover_properties_;

		double selection_diameter_;

		double bounding_rect_pad_;
};


#endif //QtPrimitiveBaseH
