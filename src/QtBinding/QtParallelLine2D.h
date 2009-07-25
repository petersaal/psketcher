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

#ifndef QtParallelLine2DH
#define QtParallelLine2DH

#include "QtPrimitiveBase.h"

class QtParallelLine2D : public QtPrimitiveBase, public ParallelLine2D
{
	public:
		QtParallelLine2D (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model);
		QtParallelLine2D (QGraphicsItem * parent,
																			 const Line2DPointer line1, const Line2DPointer line2);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

		virtual QPainterPath shape() const {return current_shape_;}

	private:
		QPainterPath current_shape_;

};
typedef boost::shared_ptr<QtParallelLine2D> QtParallelLine2DPointer;


#endif //QtParallelLine2DH
