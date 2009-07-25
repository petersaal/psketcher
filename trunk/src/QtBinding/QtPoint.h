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

#ifndef QtPointH
#define QtPointH

#include "QtPrimitiveBase.h"

// point class
class QtPoint : public QtPrimitiveBase, public Point
{
	public:
		QtPoint (QGraphicsItem * parent, double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		QtPoint (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtPoint> QtPointPointer;


#endif //QtPointH
