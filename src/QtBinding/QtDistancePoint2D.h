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

#ifndef QtDistancePoint2DH
#define QtDistancePoint2DH

#include <boost/enable_shared_from_this.hpp>

#include <QGraphicsProxyWidget>
#include "QtPrimitiveBase.h"

class QLineEdit;
class QtDistancePoint2DWidget;

class QtDistancePoint2D : public QtPrimitiveBase, public DistancePoint2D, public boost::enable_shared_from_this<QtDistancePoint2D>
{
	public:
		QtDistancePoint2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model);
		QtDistancePoint2D (QGraphicsItem * parent, const Point2DPointer point1, const Point2DPointer point2, 
						   double distance);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
		virtual QPainterPath shape() const {return current_shape_;}

		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	private:
        bool pending_db_save_;
		QtDistancePoint2DWidget *distance_widget_;
		QPainterPath current_shape_;

};
typedef boost::shared_ptr<QtDistancePoint2D> QtDistancePoint2DPointer;


// class defining the line edit graphics item used by QtDistancePoint2D
class QtDistancePoint2DWidget : public QGraphicsProxyWidget
{
	Q_OBJECT

	public:
		QtDistancePoint2DWidget(QtDistancePoint2DPointer distance_constraint, QGraphicsItem *parent = 0);
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
		virtual bool event(QEvent *event);

		void UpdateGeometry(double scale);


	signals:
		void modelChanged();  // emitted when changes are applied

	private slots:
		void textChanged();
		void applyChanges();

	private:
		// methods

		// Parameters
		QtDistancePoint2DPointer distance_constraint_; 

		QLineEdit *distance_line_edit_;
};


#endif //QtDistancePoint2DH
