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
** Copyright (C) 2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef QtDistancePointLine2DH
#define QtDistancePointLine2DH

#include <boost/enable_shared_from_this.hpp>

#include <QGraphicsProxyWidget>
#include "QtPrimitiveBase.h"

class QLineEdit;
class QtDistancePointLine2DWidget;

class QtDistancePointLine2D : public QtPrimitiveBase, public DistancePointLine2D, public boost::enable_shared_from_this<QtDistancePointLine2D>
{
	public:
		QtDistancePointLine2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model);
		QtDistancePointLine2D (QGraphicsItem * parent, const Point2DPointer point, const Line2DPointer line, 
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
		QtDistancePointLine2DWidget *distance_widget_;
		QPainterPath current_shape_;

};
typedef boost::shared_ptr<QtDistancePointLine2D> QtDistancePointLine2DPointer;


// class defining the line edit graphics item used by QtDistancePointLine2D
class QtDistancePointLine2DWidget : public QGraphicsProxyWidget
{
	Q_OBJECT

	public:
		QtDistancePointLine2DWidget(QtDistancePointLine2DPointer distance_constraint, QGraphicsItem *parent = 0);
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
		QtDistancePointLine2DPointer distance_constraint_; 

		QLineEdit *distance_line_edit_;
};


#endif //QtDistancePointLine2DH
