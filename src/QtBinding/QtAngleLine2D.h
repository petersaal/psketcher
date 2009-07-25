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

#ifndef QtAngleLine2DH
#define QtAngleLine2DH

#include <boost/enable_shared_from_this.hpp>

#include <QGraphicsProxyWidget>
#include "QtPrimitiveBase.h"

class QLineEdit;
class QtAngleLine2DWidget;

class QtAngleLine2D : public QtPrimitiveBase, public AngleLine2D, public boost::enable_shared_from_this<QtAngleLine2D>
{
	public:
		QtAngleLine2D (QGraphicsItem * parent, unsigned id, pSketcherModel &psketcher_model);
		QtAngleLine2D (QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		// Implement some of QGraphicsItem's methods
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

		virtual QPainterPath shape() const {return current_shape_;}

		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	private:
        bool pending_db_save_;
		QtAngleLine2DWidget *angle_widget_;
		
		QPainterPath current_shape_;
};
typedef boost::shared_ptr<QtAngleLine2D> QtAngleLine2DPointer;


// class defining the line edit graphics item used by QtDistancePoint2D
class QtAngleLine2DWidget : public QGraphicsProxyWidget
{
	Q_OBJECT

	public:
		QtAngleLine2DWidget(QtAngleLine2DPointer arc_primitive, QGraphicsItem *parent = 0);
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
		virtual bool event(QEvent *event);

		void UpdateGeometry(double text_s, double text_t, double scale);


	signals:
		void modelChanged();  // emitted when changes are applied

	private slots:
		void textChanged();
		void applyChanges();

	private:
		// methods

		// Parameters
		QtAngleLine2DPointer angle_constraint_primitive_; 

		QLineEdit *angle_line_edit_;
};


#endif //QtAngleLine2DH
