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

#ifndef QtCircle2DH
#define QtCircle2DH

#include <boost/enable_shared_from_this.hpp>

#include <QGraphicsProxyWidget>
#include "QtPrimitiveBase.h"

class QLineEdit;
class QtCircle2DWidget;

// line class
class QtCircle2D : public boost::enable_shared_from_this<QtCircle2D>, public QtPrimitiveBase, public Circle2D
{
	public:
		QtCircle2D (QGraphicsItem * parent, double s_center, double t_center, double radius, 
			      SketchPlanePointer sketch_plane, bool s_center_free = false, bool t_center_free = false, bool radius_free = false);
		QtCircle2D (QGraphicsItem * parent, double s1, double t1, double s2, double t2, double s3, double t3,
			      SketchPlanePointer sketch_plane, bool s_center_free = false, bool t_center_free = false, bool radius_free = false);
		QtCircle2D (QGraphicsItem * parent,DOFPointer s_center, DOFPointer t_center, DOFPointer radius, SketchPlanePointer sketch_plane);
		QtCircle2D (QGraphicsItem * parent, unsigned id, Ark3DModel &ark3d_model); // Construct from database

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
		QtCircle2DWidget *radius_widget_;
		QPainterPath current_shape_;

};
typedef boost::shared_ptr<QtCircle2D> QtCircle2DPointer;


// class defining the line edit graphics item used by QtDistancePoint2D
class QtCircle2DWidget : public QGraphicsProxyWidget
{
	Q_OBJECT

	public:
		QtCircle2DWidget(QtCircle2DPointer circle_primitive, QGraphicsItem *parent = 0);
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
		QtCircle2DPointer circle_primitive_; 

		QLineEdit *radius_line_edit_;
};

#endif //QtCircle2DH
