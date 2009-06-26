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

#ifndef QtPoint2DH
#define QtPoint2DH

#include "QtPrimitiveBase.h"

// point class
class QtPoint2D : public QtPrimitiveBase, public Point2D
{
	public:
		QtPoint2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model);
		QtPoint2D (QGraphicsItem * parent,double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		QtPoint2D (QGraphicsItem * parent, DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}
		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

		virtual QPainterPath shape() const {return current_shape_;}

		virtual void mouseMoveEvent    ( QGraphicsSceneMouseEvent * event );
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	private:
		QPainterPath current_shape_;

		bool pending_db_save_;
};
typedef boost::shared_ptr<QtPoint2D> QtPoint2DPointer;


#endif //QtPoint2DH
