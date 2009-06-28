/****************************************************************************
**
** This file is part of the QtOpenCascade Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) Peter Dolbey 2006. All rights reserved.
**
** Adapted for use as part of Ark3D by Michael Greminger on 3-1-08
**
****************************************************************************/

#include <QtGui/QtGui>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

#include "Ark3DWindow.h"


Ark3DWindow::Ark3DWindow()
: myLastFolder(tr(""))
{
	// @fixme will need a way to automate setting the size of the graphics scene as primitives are added and removed
	scene = new QGraphicsScene(-20, -20, 40, 40);

	view = new Ark3DWidget(scene,this); //Note this has changed!
	this->setCentralWidget(view);

	connect(view, SIGNAL(sketchActionFinished()), this, SLOT(triggerSketchActionGroup()));  // after sketch action is triggered, retrigger the currently selected action

    createActions();
    createMenus();
	createToolBars();

    statusBar()->showMessage(tr("A context menu is available by right-clicking"));

    setWindowTitle(tr("Ark3D"));
    setMinimumSize(160, 160);
	showMaximized();
}

void Ark3DWindow::about()
{
    statusBar()->showMessage(tr("Invoked Help|About"));
    QMessageBox::about(this, tr("About Menu"),
            tr("<h2>Ark3D</h2> <p><A HREF=\"http://sourceforge.net/projects/ark3d/\">http://sourceforge.net/projects/ark3d/</A>"));
}


void Ark3DWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	// Now for the QtOCCViewWidget slots.

	fitAction = new QAction(tr("&Fit Window"), this);
	fitAction->setIcon(QIcon(":/icons/zoom_fit.svg"));
	fitAction->setShortcut(tr("Ctrl+F"));
    fitAction->setStatusTip(tr("Fit to window"));
    connect(fitAction, SIGNAL(triggered()), view, SLOT(fitExtents()));

	zoomAction = new QAction(tr("&Zoom"), this);
	zoomAction->setIcon(QIcon(":/icons/zoom_box.svg"));
	zoomAction->setStatusTip(tr("Zoom in window"));
    connect(zoomAction, SIGNAL(triggered()), view, SLOT(fitArea()));

	// actions for testing
	// generate test sketch
	generateSketchAction = new QAction(tr("Generate Test Sketch"), this);
	generateSketchAction->setStatusTip(tr("Generate Test Sketch"));
    connect(generateSketchAction, SIGNAL(triggered()), view, SLOT(GenerateTestSketch()));

	// Solve Constraints
	solveConstraintsAction = new QAction(tr("Solve Constraints"), this);
	solveConstraintsAction->setStatusTip(tr("Solve Constraints"));
    connect(solveConstraintsAction, SIGNAL(triggered()), view, SLOT(SolveConstraints()));

	// file actions
	newFileAction = new QAction(tr("New"), this);
	newFileAction->setStatusTip(tr("Create New File"));
	connect(newFileAction, SIGNAL(triggered()), view, SLOT(newFile()));

	openAction = new QAction(tr("Open..."), this);
	openAction->setStatusTip(tr("Open an Existing File"));
	connect(openAction, SIGNAL(triggered()), view, SLOT(open()));

	saveAction = new QAction(tr("Save"), this);
	saveAction->setStatusTip(tr("Save Current File"));
	connect(saveAction, SIGNAL(triggered()), view, SLOT(save()));

	saveAsAction = new QAction(tr("Save As..."), this);
	saveAsAction->setStatusTip(tr("Save Current File Under a Different Name"));
	connect(saveAsAction, SIGNAL(triggered()), view, SLOT(saveAs()));

	exportDXFAction = new QAction(tr("Export DXF..."), this);
	exportDXFAction->setStatusTip(tr("Export DXF File"));
	connect(exportDXFAction, SIGNAL(triggered()), view, SLOT(exportDXF()));

	// edit actions
	undoAction = new QAction(tr("Undo"), this);
	undoAction->setShortcut(tr("Ctrl+Z"));
	undoAction->setStatusTip(tr("Undo"));
	connect(undoAction, SIGNAL(triggered()), view, SLOT(undo()));

	redoAction = new QAction(tr("Redo"), this);
	redoAction->setShortcut(tr("Ctrl+Shift+Z"));
	redoAction->setStatusTip(tr("Redo"));
	connect(redoAction, SIGNAL(triggered()), view, SLOT(redo()));

	// Create Sketch actions
	sketchActionGroup = new QActionGroup(this);
	
	selectAction = new QAction(tr("&Select"), sketchActionGroup);
	selectAction->setCheckable(true);
	selectAction->setIcon(QIcon(":/icons/select_pointer.svg"));
	selectAction->setStatusTip(tr("Select"));
    connect(selectAction, SIGNAL(triggered()), view, SLOT(select()));

	makeLineAction = new QAction(tr("Line"), sketchActionGroup);
	makeLineAction->setCheckable(true);
	makeLineAction->setIcon(QIcon(":/icons/line.svg"));
	makeLineAction->setStatusTip(tr("Create Line"));
    connect(makeLineAction, SIGNAL(triggered()), view, SLOT(MakeLine()));

	makePointAction = new QAction(tr("Point"), sketchActionGroup);
	makePointAction->setCheckable(true);
	makePointAction->setIcon(QIcon(":/icons/point.svg"));
	makePointAction->setStatusTip(tr("Create Point"));
    connect(makePointAction, SIGNAL(triggered()), view, SLOT(MakePoint()));

	makeArcAction = new QAction(tr("Arc"), sketchActionGroup);
	makeArcAction->setCheckable(true);
	makeArcAction->setIcon(QIcon(":/icons/arc.svg"));
	makeArcAction->setStatusTip(tr("Create Arc"));
    connect(makeArcAction, SIGNAL(triggered()), view, SLOT(MakeArc()));

    makeCircleAction = new QAction(tr("Circle"), sketchActionGroup);
    makeCircleAction->setCheckable(true);
    makeCircleAction->setIcon(QIcon(":/icons/Circle.svg"));
    makeCircleAction->setStatusTip(tr("Create Circle"));
    connect(makeCircleAction, SIGNAL(triggered()), view, SLOT(MakeCircle()));

	makeDistanceConstraintAction = new QAction(tr("Distance Constraint"), sketchActionGroup);
	makeDistanceConstraintAction->setCheckable(true);
	makeDistanceConstraintAction->setIcon(QIcon(":/icons/distance.svg"));
	makeDistanceConstraintAction->setStatusTip(tr("Create Distance Constraint"));
    connect(makeDistanceConstraintAction, SIGNAL(triggered()), view, SLOT(MakeDistanceConstraint()));

	makeAngleConstraintAction = new QAction(tr("Angle Constraint"), sketchActionGroup);
	makeAngleConstraintAction->setCheckable(true);
	makeAngleConstraintAction->setIcon(QIcon(":/icons/angle.svg"));
	makeAngleConstraintAction->setStatusTip(tr("Create Angle Constraint"));
    connect(makeAngleConstraintAction, SIGNAL(triggered()), view, SLOT(MakeAngleConstraint()));

	makeParallelConstraintAction = new QAction(tr("Parallel Constraint"), sketchActionGroup);
	makeParallelConstraintAction->setCheckable(true);
	makeParallelConstraintAction->setIcon(QIcon(":/icons/parallel.svg"));
	makeParallelConstraintAction->setStatusTip(tr("Create Parallelism Constraint"));
    connect(makeParallelConstraintAction, SIGNAL(triggered()), view, SLOT(MakeParallelConstraint()));

	makeHorizontalConstraintAction = new QAction(tr("Horizontal Constraint"), sketchActionGroup);
	makeHorizontalConstraintAction->setCheckable(true);
	makeHorizontalConstraintAction->setIcon(QIcon(":/icons/horizontal.svg"));
	makeHorizontalConstraintAction->setStatusTip(tr("Create Horizontal Constraint"));
    connect(makeHorizontalConstraintAction, SIGNAL(triggered()), view, SLOT(MakeHorizontalConstraint()));

	makeVerticalConstraintAction = new QAction(tr("Vertical Constraint"), sketchActionGroup);
	makeVerticalConstraintAction->setCheckable(true);
	makeVerticalConstraintAction->setIcon(QIcon(":/icons/vertical.svg"));
	makeVerticalConstraintAction->setStatusTip(tr("Create Vertical Constraint"));
    connect(makeVerticalConstraintAction, SIGNAL(triggered()), view, SLOT(MakeVerticalConstraint()));

	makeTangentConstraintAction = new QAction(tr("Tangent Constraint"), sketchActionGroup);
	makeTangentConstraintAction->setCheckable(true);
	makeTangentConstraintAction->setIcon(QIcon(":/icons/tangent.svg"));
	makeTangentConstraintAction->setStatusTip(tr("Create Tangency Constraint"));
    connect(makeTangentConstraintAction, SIGNAL(triggered()), view, SLOT(MakeTangentConstraint()));

	selectAction->setChecked(true);
}

void Ark3DWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File") );
		fileMenu->addAction( newFileAction );
		fileMenu->addAction( openAction );
		fileMenu->addAction( saveAction );
		fileMenu->addAction( saveAsAction );
		fileMenu->addAction( exportDXFAction );
		fileMenu->addAction( exitAction );

    editMenu = menuBar()->addMenu( tr("&Edit") );
		editMenu->addAction( undoAction );
		editMenu->addAction( redoAction );

	viewMenu = menuBar()->addMenu( tr("&View") );
			viewMenu->addAction( fitAction );
			viewMenu->addAction( zoomAction );

   helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void Ark3DWindow::createToolBars()
{
	viewToolBar = addToolBar(tr("&View"));

	viewToolBar->addAction(fitAction);
	viewToolBar->addAction(zoomAction);

	// create the tool bar containing debugging actions
	debugToolBar = addToolBar(tr("Debug"));
	debugToolBar->addAction(generateSketchAction);
	debugToolBar->addAction(solveConstraintsAction);

	// create the sketch tool bar
	sketchToolBar = addToolBar(tr("Sketch"));
	sketchToolBar->addActions(sketchActionGroup->actions());
}

// retrigger current active button in the sketch action group
void Ark3DWindow::triggerSketchActionGroup()
{
	QAction *current_action = sketchActionGroup->checkedAction();
	current_action->toggle();
	current_action->trigger();
}

void Ark3DWindow::switchToSelectionMode()
{
	selectAction->setChecked(true);
}


