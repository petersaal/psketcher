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

#include <Aspect_RectangularGrid.hxx>
#include <V3d_View.hxx>

#include "Ark3DWindow.h"


Ark3DWindow::Ark3DWindow()
: myLastFolder(tr(""))
{
    myVC  = new QoccViewerContext();
	myOCC = new Ark3DWidget(myVC->getContext(), this); //Note this has changed!
	this->setCentralWidget(myOCC);
	connect(myOCC, SIGNAL(sketchActionFinished()), this, SLOT(triggerSketchActionGroup()));  // after sketch action is triggered, retrigger the currently selected action
	connect(myOCC, SIGNAL(modeChanged()), this, SLOT(viewerModeChanged()));  // if viewer mode is change, may need to reset selection mode



    createActions();
    createMenus();
	createToolBars();

    statusBar()->showMessage(tr("A context menu is available by right-clicking"));

    setWindowTitle(tr("Ark3D"));
    setMinimumSize(160, 160);
	showMaximized();
}

void Ark3DWindow::newFile()
{
    statusBar()->showMessage(tr("Invoked File|New"));
	//myOCC->getView()->ColorScaleErase();
	myVC->deleteAllObjects();
}

void Ark3DWindow::open()
{

}

void Ark3DWindow::save()
{
    statusBar()->showMessage(tr("Invoked File|Save"));
}

void Ark3DWindow::print()
{
    statusBar()->showMessage(tr("Invoked File|Print"));
}

void Ark3DWindow::undo()
{
    statusBar()->showMessage(tr("Invoked Edit|Undo"));
}

void Ark3DWindow::redo()
{
    statusBar()->showMessage(tr("Invoked Edit|Redo"));
}

void Ark3DWindow::cut()
{
    statusBar()->showMessage(tr("Invoked Edit|Cut"));
}

void Ark3DWindow::copy()
{
    statusBar()->showMessage(tr("Invoked Edit|Copy"));
}

void Ark3DWindow::paste()
{
    statusBar()->showMessage(tr("Invoked Edit|Paste"));
}

void Ark3DWindow::about()
{
    statusBar()->showMessage(tr("Invoked Help|About"));
    QMessageBox::about(this, tr("About Menu"),
            tr("<h2>Ark3D</h2> <p><A HREF=\"http://sourceforge.net/projects/ark3d/\">http://sourceforge.net/projects/ark3d/</A>"));
}


void Ark3DWindow::xyzPosition (V3d_Coordinate X,
							  V3d_Coordinate Y,
							  V3d_Coordinate Z)
{
	QString aString;
	QTextStream ts(&aString);
	ts << X << "," << Y << "," << Z;
	statusBar()->showMessage(aString);
}

void Ark3DWindow::addPoint (V3d_Coordinate X,
						   V3d_Coordinate Y,
						   V3d_Coordinate Z)
{

}

void Ark3DWindow::statusMessage (const QString aMessage)
{
	statusBar()->showMessage(aMessage);
}


void Ark3DWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save the document to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    printAction = new QAction(tr("&Print..."), this);
    printAction->setShortcut(tr("Ctrl+P"));
    printAction->setStatusTip(tr("Print the document"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Undo the last operation"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+Y"));
    redoAction->setStatusTip(tr("Redo the last operation"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	// Now for the QtOCCViewWidget slots.

	fitAction = new QAction(tr("&Fit Window"), this);
	fitAction->setIcon(QIcon(":/icons/zoom_fit.svg"));
	fitAction->setShortcut(tr("Ctrl+F"));
    fitAction->setStatusTip(tr("Fit to window"));
    connect(fitAction, SIGNAL(triggered()), myOCC, SLOT(fitExtents()));

	fitAllAction = new QAction(tr("Fit &All"), this);
	fitAllAction->setIcon(QIcon(":/icons/zoom_fit.svg"));
	fitAllAction->setShortcut(tr("Ctrl+A"));
    fitAllAction->setStatusTip(tr("Fit contents to viewport"));
    connect(fitAllAction, SIGNAL(triggered()), myOCC, SLOT(fitAll()));

	zoomAction = new QAction(tr("&Zoom"), this);
	zoomAction->setIcon(QIcon(":/icons/zoom_box.svg"));
	zoomAction->setStatusTip(tr("Zoom in window"));
    connect(zoomAction, SIGNAL(triggered()), myOCC, SLOT(fitArea()));

	panAction = new QAction(tr("&Pan"), this);
    panAction->setStatusTip(tr("Window panning"));
    connect(panAction, SIGNAL(triggered()), myOCC, SLOT(pan()));

	rotAction = new QAction(tr("&Rotate"), this);
	rotAction->setShortcut(tr("Ctrl+R"));
    rotAction->setStatusTip(tr("Window rotation"));
    connect(rotAction, SIGNAL(triggered()), myOCC, SLOT(rotation()));

	gridOnAction = new QAction(tr("&Grid On"), this);
	gridOnAction->setShortcut(tr("Ctrl+G"));
    gridOnAction->setStatusTip(tr("Turn the grid on"));
    connect(gridOnAction, SIGNAL(triggered()), myVC, SLOT(gridOn()));

	gridOffAction = new QAction(tr("Gri&d Off"), this);
	gridOffAction->setShortcut(tr("Ctrl+D"));
    gridOffAction->setStatusTip(tr("Turn the grid on"));
    connect(gridOffAction, SIGNAL(triggered()), myVC, SLOT(gridOff()));

	gridXYAction = new QAction(tr("XY Grid"), this);
    gridXYAction->setStatusTip(tr("Grid on XY Plane"));
	//gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridXYAction, SIGNAL(triggered()), myVC, SLOT(gridXY()));

	gridXZAction = new QAction(tr("XZ Grid"), this);
    gridXZAction->setStatusTip(tr("Grid on XZ Plane"));
	//gridXZAction->setShortcut(tr("Ctrl+Y"));
    connect(gridXZAction, SIGNAL(triggered()), myVC, SLOT(gridXZ()));

	gridYZAction = new QAction(tr("YZ Grid"), this);
	gridYZAction->setStatusTip(tr("Grid on YZ Plane"));
	//gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridYZAction, SIGNAL(triggered()), myVC, SLOT(gridYZ()));

	gridRectAction = new QAction(tr("Rectangular"), this);
	gridRectAction->setStatusTip(tr("Retangular grid"));
	//gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridRectAction, SIGNAL(triggered()), myVC, SLOT(gridRect()));

	gridCircAction = new QAction(tr("Circular"), this);
	gridCircAction->setStatusTip(tr("Circular grid"));
	//gridOffAction->setShortcut(tr("Ctrl+Z"));
    connect(gridCircAction, SIGNAL(triggered()), myVC, SLOT(gridCirc()));

	// Shading and wireframe actions
    viewModeActionGroup = new QActionGroup(this);

	viewShadedAction = new QAction(tr("Shaded"), viewModeActionGroup);
	viewShadedAction->setCheckable(true);
	viewShadedAction->setIcon(QIcon(":/icons/shaded.svg"));
    viewShadedAction->setStatusTip(tr("Shade solids"));
    connect(viewShadedAction, SIGNAL(triggered()), myOCC, SLOT(viewShaded()));

	viewWireframeAction = new QAction(tr("Wire frame"), viewModeActionGroup);
	viewWireframeAction->setCheckable(true);
	viewWireframeAction->setIcon(QIcon(":/icons/wire_frame.svg"));
    viewWireframeAction->setStatusTip(tr("View solids in wire frame"));
    connect(viewWireframeAction, SIGNAL(triggered()), myOCC, SLOT(viewWireFrame()));

	viewNoHiddenLineAction = new QAction(tr("No hidden lines"), viewModeActionGroup);
	viewNoHiddenLineAction->setCheckable(true);
	viewNoHiddenLineAction->setIcon(QIcon(":/icons/wire_frame_no_hidden.svg"));
    viewNoHiddenLineAction->setStatusTip(tr("View solids in wire frame with no hidden lines"));
    connect(viewNoHiddenLineAction, SIGNAL(triggered()), myOCC, SLOT(viewNoHiddenLine()));

    viewShadedAction->setChecked(true);

	// Standard View
	viewFrontAction = new QAction(tr("Front"), this);
	viewFrontAction->setIcon(QIcon(":/icons/front_view.svg"));
	viewFrontAction->setStatusTip(tr("View From Front"));
    connect(viewFrontAction, SIGNAL(triggered()), myOCC, SLOT(viewFront()));

	viewBackAction = new QAction(tr("Back"), this);
	viewBackAction->setIcon(QIcon(":/icons/back_view.svg"));
	viewBackAction->setStatusTip(tr("View From Back"));
    connect(viewBackAction, SIGNAL(triggered()), myOCC, SLOT(viewBack()));

	viewTopAction = new QAction(tr("Top"), this);
	viewTopAction->setIcon(QIcon(":/icons/top_view.svg"));
	viewTopAction->setStatusTip(tr("View From Top"));
    connect(viewTopAction, SIGNAL(triggered()), myOCC, SLOT(viewTop()));

	viewBottomAction = new QAction(tr("Bottom"), this);
	viewBottomAction->setIcon(QIcon(":/icons/bottom_view.svg"));
	viewBottomAction->setStatusTip(tr("View From Bottom"));
    connect(viewBottomAction, SIGNAL(triggered()), myOCC, SLOT(viewBottom()));

	viewLeftAction = new QAction(tr("Left"), this);
	viewLeftAction->setIcon(QIcon(":/icons/left_view.svg"));
	viewLeftAction->setStatusTip(tr("View From Left"));
    connect(viewLeftAction, SIGNAL(triggered()), myOCC, SLOT(viewLeft()));

	viewRightAction = new QAction(tr("Right"), this);
	viewRightAction->setIcon(QIcon(":/icons/right_view.svg"));
	viewRightAction->setStatusTip(tr("View From Right"));
    connect(viewRightAction, SIGNAL(triggered()), myOCC, SLOT(viewRight()));

	viewAxoAction = new QAction(tr("Axonometric"), this);
	viewAxoAction->setStatusTip(tr("Axonometric view"));
    connect(viewAxoAction, SIGNAL(triggered()), myOCC, SLOT(viewAxo()));

	viewGridAction = new QAction(tr("Grid"), this);
	viewGridAction->setStatusTip(tr("View from grid"));
    connect(viewGridAction, SIGNAL(triggered()), myOCC, SLOT(viewGrid()));

	viewResetAction = new QAction(tr("Reset"), this);
	viewResetAction->setStatusTip(tr("Reset the view"));
    connect(viewResetAction, SIGNAL(triggered()), myOCC, SLOT(viewReset()));

	backgroundAction = new QAction( tr("&Background"), this );
	backgroundAction->setStatusTip(tr("Change the background colour"));
	connect(backgroundAction, SIGNAL(triggered()), myOCC, SLOT(background()));
	
	// The co-ordinates from the view
	connect( myOCC, SIGNAL(mouseMoved(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)),
		     this,   SLOT(xyzPosition(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)) );

	// Add a point from the view
	connect( myOCC, SIGNAL(pointClicked(V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)),
		     this,   SLOT (addPoint    (V3d_Coordinate,V3d_Coordinate,V3d_Coordinate)) );

	connect( myOCC, SIGNAL(sendStatus(const QString)),
		     this,  SLOT  (statusMessage(const QString)) );

	// actions for testing
	// generate test sketch
	generateSketchAction = new QAction(tr("Generate Test Sketch"), this);
	generateSketchAction->setStatusTip(tr("Generate Test Sketch"));
    connect(generateSketchAction, SIGNAL(triggered()), myOCC, SLOT(GenerateTestSketch()));

	// Solve Constraints
	solveConstraintsAction = new QAction(tr("Solve Constraints"), this);
	solveConstraintsAction->setStatusTip(tr("Solve Constraints"));
    connect(solveConstraintsAction, SIGNAL(triggered()), myOCC, SLOT(SolveConstraints()));


	// Create Sketch actions
	sketchActionGroup = new QActionGroup(this);
	
	selectAction = new QAction(tr("&Select"), sketchActionGroup);
	selectAction->setCheckable(true);
	selectAction->setIcon(QIcon(":/icons/select_pointer.svg"));
	selectAction->setStatusTip(tr("Select"));
    connect(selectAction, SIGNAL(triggered()), myOCC, SLOT(select()));

	makeLineAction = new QAction(tr("Line"), sketchActionGroup);
	makeLineAction->setCheckable(true);
	makeLineAction->setIcon(QIcon(":/icons/line.svg"));
	makeLineAction->setStatusTip(tr("Create Line"));
    connect(makeLineAction, SIGNAL(triggered()), myOCC, SLOT(MakeLine()));

	makePointAction = new QAction(tr("Point"), sketchActionGroup);
	makePointAction->setCheckable(true);
	makePointAction->setIcon(QIcon(":/icons/point.svg"));
	makePointAction->setStatusTip(tr("Create Point"));
    connect(makePointAction, SIGNAL(triggered()), myOCC, SLOT(MakePoint()));

	makeDistanceConstraintAction = new QAction(tr("Distance Constraint"), sketchActionGroup);
	makeDistanceConstraintAction->setCheckable(true);
	makeDistanceConstraintAction->setIcon(QIcon(":/icons/distance.svg"));
	makeDistanceConstraintAction->setStatusTip(tr("Create Distance Constraint"));
    connect(makeDistanceConstraintAction, SIGNAL(triggered()), myOCC, SLOT(MakeDistanceConstraint()));

	makeAngleConstraintAction = new QAction(tr("Angle Constraint"), sketchActionGroup);
	makeAngleConstraintAction->setCheckable(true);
	makeAngleConstraintAction->setIcon(QIcon(":/icons/angle.svg"));
	makeAngleConstraintAction->setStatusTip(tr("Create Angle Constraint"));
    connect(makeAngleConstraintAction, SIGNAL(triggered()), myOCC, SLOT(MakeAngleConstraint()));

	selectAction->setChecked(true);
}

void Ark3DWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File") );
		fileMenu->addAction( newAction );
		fileMenu->addAction( openAction );
		fileMenu->addAction( saveAction );
		fileMenu->addAction( printAction );


		fileMenu->addSeparator();
		fileMenu->addAction( exitAction );

    editMenu = menuBar()->addMenu( tr("&Edit") );
		editMenu->addAction( undoAction );
		editMenu->addAction( redoAction );
		editMenu->addSeparator();
		editMenu->addAction( cutAction );
		editMenu->addAction( copyAction );
		editMenu->addAction( pasteAction );

	viewMenu = menuBar()->addMenu( tr("&View") );
		viewDisplayMenu = viewMenu->addMenu( tr("&Display") );
			viewDisplayMenu->addAction( viewFrontAction );
			viewDisplayMenu->addAction( viewBackAction );
			viewDisplayMenu->addAction( viewTopAction );
			viewDisplayMenu->addAction( viewBottomAction );
			viewDisplayMenu->addAction( viewLeftAction );
			viewDisplayMenu->addAction( viewRightAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( viewAxoAction );
			viewDisplayMenu->addAction( viewGridAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( viewResetAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addAction( backgroundAction );
			viewDisplayMenu->addSeparator();
			viewDisplayMenu->addActions(viewModeActionGroup->actions());

		viewActionsMenu = viewMenu->addMenu( tr("&Actions") );
			viewActionsMenu->addAction( fitAction );
			viewActionsMenu->addAction( fitAllAction );
			viewActionsMenu->addAction( zoomAction );
			viewActionsMenu->addAction( panAction );
			viewActionsMenu->addAction( rotAction );

		gridMenu = viewMenu->addMenu( tr("&Grid") );
			gridMenu->addAction( gridOnAction );
			gridMenu->addAction( gridOffAction );
			gridMenu->addSeparator();
			gridMenu->addAction( gridXYAction );
			gridMenu->addAction( gridXZAction );
			gridMenu->addAction( gridYZAction );
			gridMenu->addSeparator();
			gridMenu->addAction( gridRectAction );
			gridMenu->addAction( gridCircAction );
			
   helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void Ark3DWindow::createToolBars()
{
	viewToolBar = addToolBar(tr("&View"));
	//viewToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	viewToolBar->addAction(viewFrontAction);
	viewToolBar->addAction(viewBackAction);
	viewToolBar->addAction(viewTopAction);
	viewToolBar->addAction(viewBottomAction);
	viewToolBar->addAction(viewLeftAction);
	viewToolBar->addAction(viewRightAction);
	viewToolBar->addSeparator();
	viewToolBar->addAction(fitAction);
	viewToolBar->addAction(zoomAction);
	viewToolBar->addSeparator();
	viewToolBar->addActions(viewModeActionGroup->actions());

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

// if qoccviewwidget mode is CurAction3d_Nothing and selection action button is active, set qoccviewwidget mode to CurAction3d_Picking
void Ark3DWindow::viewerModeChanged()
{
	
	if(myOCC->getMode() == CurAction3d_Nothing)
		if(selectAction->isChecked())
			{
				myOCC->select();
			}
	
}
