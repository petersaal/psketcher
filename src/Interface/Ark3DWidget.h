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

#ifndef ARK3DWIDGET_H
#define ARK3DWIDGET_H

/*
extern "C" {
#include <Python.h>
}
*/

#include <QGraphicsView>

#include "../QtBinding/QtSketch.h"
#include "../InteractiveConstructors/InteractiveConstructorBase.h"
#include "../InteractiveConstructors/Point2DConstructor.h"
#include "../InteractiveConstructors/Line2DConstructor.h"
#include "../InteractiveConstructors/Distance2DConstructor.h"
#include "../InteractiveConstructors/AngleLine2DConstructor.h"
#include "../InteractiveConstructors/ParallelLine2DConstructor.h"
#include "../InteractiveConstructors/TangentEdge2DConstructor.h"
#include "../InteractiveConstructors/Arc2DConstructor.h"
#include "../InteractiveConstructors/HoriVertLine2DConstructor.h"

class QGraphicsScene;

class Ark3DWidget : public QGraphicsView
{
	Q_OBJECT

	public:
		Ark3DWidget(QGraphicsScene * scene, QWidget * parent = 0);
		~Ark3DWidget();
	
		void GenerateDefaultSketch();

	
	signals:
		void sketchActionFinished();
	
	public slots:
		void GenerateTestSketch();
		void ExecutePythonScript();
		void SolveConstraints();

		void select(); // overides the select solot for QoccViewWidget

		// Interactive constructor slots
		void MakeLine();
		void MakeHorizontalConstraint();
		void MakeVerticalConstraint();
		void MakePolyLine();
		void MakeArc();
		void MakePoint();
		void MakeDistanceConstraint();
		void MakeAngleConstraint();
		void MakeTangentConstraint();
		void MakeParallelConstraint();

		void fitExtents();

		// slots for file handling
		void open();
		void newFile();
		bool save();
		bool saveAs();
		bool exportDXF();

		// slots for undo/redo
		void modelChanged(QString description);
		void undo();	
		void redo();
	
	protected: // methods
		virtual void paintEvent        ( QPaintEvent* e );
		virtual void resizeEvent       ( QResizeEvent* e );
		virtual void mousePressEvent   ( QMouseEvent* e );
		virtual void mouseReleaseEvent ( QMouseEvent* e );
		virtual void mouseMoveEvent    ( QMouseEvent* e );
		virtual void wheelEvent        ( QWheelEvent* e );
		virtual void leaveEvent		   ( QEvent * e);
		virtual void mouseDoubleClickEvent ( QMouseEvent * event );
		virtual void drawBackground ( QPainter * painter, const QRectF & rect );
		virtual void keyReleaseEvent ( QKeyEvent * event );

		void SelectUnderMouse(QMouseEvent *e, bool multi_select = false);
	
	private: // members
		QtSketchPointer current_sketch_;
		InteractiveConstructorBase *interactive_primitive_;
	
	
	private: // methods
		QPoint previous_mouse_position_;
	
	
	public:

};

#endif // ARK3DWIDGET_H
