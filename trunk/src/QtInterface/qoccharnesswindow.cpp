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
****************************************************************************/

#include <QtGui/QtGui>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

#include <Aspect_RectangularGrid.hxx>
#include <V3d_View.hxx>

#include "qoccharnesswindow.h"

void LoadBottle ( Handle_AIS_InteractiveContext theContext );
void ShowOrigin ( Handle_AIS_InteractiveContext theContext );
void AddVertex  ( double x, double y, double z, Handle_AIS_InteractiveContext theContext );

QoccHarnessWindow::QoccHarnessWindow()
: myLastFolder(tr(""))
{
    myVC  = new QoccViewerContext();
	myOCC = new QoccViewWidget(myVC->getContext(), this); //Note this has changed!
	this->setCentralWidget(myOCC);
/*
	ShowOrigin ( myVC->getContext() );

	AddVertex(   0.0, 100.0,   0.0, myVC->getContext());
	AddVertex( 100.0, 100.0,   0.0, myVC->getContext());
	AddVertex( 100.0,   0.0,   0.0, myVC->getContext());
*/

	//LoadBottle( myVC->getContext() );

    createActions();
    createMenus();
	createToolBars();

    statusBar()->showMessage(tr("A context menu is available by right-clicking"));

    setWindowTitle(tr("Qt OpenCASCADE"));
    setMinimumSize(160, 160);
	showMaximized();
}

void QoccHarnessWindow::newFile()
{
    statusBar()->showMessage(tr("Invoked File|New"));
	//myOCC->getView()->ColorScaleErase();
	myVC->deleteAllObjects();
}

void QoccHarnessWindow::open()
{
	QString		fileName;
	QString		fileType;
	QFileInfo	fileInfo;

	QoccInputOutput::FileFormat format;
	QoccInputOutput reader;

    statusBar()->showMessage(tr("Invoked File|Open"));

	fileName = QFileDialog::getOpenFileName (	this,
				  								tr("Open File"),
												myLastFolder,
												tr( "All drawing types (*.brep *.rle *.igs *iges *.stp *.step);;"
													"BREP (*.brep *.rle);;"
													"STEP (*.step *.stp);;"
													"IGES (*.iges *.igs)" ) );
	if (!fileName.isEmpty())
	{
		fileInfo.setFile(fileName);
		fileType = fileInfo.suffix();
		if (fileType.toLower() == tr("brep") || fileType.toLower() == tr("rle"))
		{
			format = QoccInputOutput::FormatBREP;
		}
		if (fileType.toLower() == tr("step") || fileType.toLower() == tr("stp"))
		{
			format = QoccInputOutput::FormatSTEP;
		}
		if (fileType.toLower() == tr("iges") || fileType.toLower() == tr("igs"))
		{
			format = QoccInputOutput::FormatIGES;
		}
		myLastFolder = fileInfo.absolutePath();
		reader.importModel ( fileInfo.absoluteFilePath(), format, myOCC->getContext() );
	}

	myOCC->fitAll();

}

void QoccHarnessWindow::save()
{
    statusBar()->showMessage(tr("Invoked File|Save"));
}

void QoccHarnessWindow::print()
{
    statusBar()->showMessage(tr("Invoked File|Print"));
}

void QoccHarnessWindow::undo()
{
    statusBar()->showMessage(tr("Invoked Edit|Undo"));
}

void QoccHarnessWindow::redo()
{
    statusBar()->showMessage(tr("Invoked Edit|Redo"));
}

void QoccHarnessWindow::cut()
{
    statusBar()->showMessage(tr("Invoked Edit|Cut"));
}

void QoccHarnessWindow::copy()
{
    statusBar()->showMessage(tr("Invoked Edit|Copy"));
}

void QoccHarnessWindow::paste()
{
    statusBar()->showMessage(tr("Invoked Edit|Paste"));
}

void QoccHarnessWindow::about()
{
    statusBar()->showMessage(tr("Invoked Help|About"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Qt OpenCASCADE</b> example shows how to create a simple Qt4 Viewer."));
}

void QoccHarnessWindow::aboutQt()
{
    statusBar()->showMessage(tr("Invoked Help|About Qt"));
}

void QoccHarnessWindow::bottle()
{
    statusBar()->showMessage(tr("Invoked File|Load Bottle"));
	QApplication::setOverrideCursor( Qt::WaitCursor );
	myVC->deleteAllObjects ();
	// Call the "bottle factory" to load and display the shape
	LoadBottle (myVC->getContext());
	// Force redraw
	myOCC->getView()->Redraw();
	QApplication::restoreOverrideCursor();
}

void QoccHarnessWindow::xyzPosition (V3d_Coordinate X,
							  V3d_Coordinate Y,
							  V3d_Coordinate Z)
{
	QString aString;
	QTextStream ts(&aString);
	ts << X << "," << Y << "," << Z;
	statusBar()->showMessage(aString);
}

void QoccHarnessWindow::addPoint (V3d_Coordinate X,
						   V3d_Coordinate Y,
						   V3d_Coordinate Z)
{
	AddVertex ( X, Y, Z, myVC->getContext() );
}

void QoccHarnessWindow::statusMessage (const QString aMessage)
{
	statusBar()->showMessage(aMessage);
}


void QoccHarnessWindow::createActions()
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

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(aboutQt()));

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

	selectAction = new QAction(tr("&Select"), this);
	zoomAction->setStatusTip(tr("Select"));
    connect(selectAction, SIGNAL(triggered()), myOCC, SLOT(select()));

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

	// And the bottle example

	bottleAction = new QAction(tr("Load &Bottle"), this);
	bottleAction->setShortcut(tr("Ctrl+B"));
    bottleAction->setStatusTip(tr("Bottle sample."));
    connect(bottleAction, SIGNAL(triggered()), this, SLOT(bottle()));

}

void QoccHarnessWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File") );
		fileMenu->addAction( newAction );
		fileMenu->addAction( openAction );
		fileMenu->addAction( saveAction );
		fileMenu->addAction( printAction );
/*
		Comment out the 2 lines below to hide the
		Load Bottle menu option - still left in for
		now as a demo feature.
*/
		fileMenu->addSeparator();
		fileMenu->addAction( bottleAction );

		fileMenu->addSeparator();
		fileMenu->addAction( exitAction );

    editMenu = menuBar()->addMenu( tr("&Edit") );
		editMenu->addAction( selectAction );
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
    helpMenu->addAction(aboutQtAction);
}

void QoccHarnessWindow::createToolBars()
{
	viewToolBar = addToolBar(tr("&View"));
	viewToolBar->addAction(viewFrontAction);
	viewToolBar->addAction(viewBackAction);
	viewToolBar->addAction(viewTopAction);
	viewToolBar->addAction(viewBottomAction);
	viewToolBar->addAction(viewLeftAction);
	viewToolBar->addAction(viewRightAction);
	viewToolBar->addAction(fitAction);
	viewToolBar->addAction(zoomAction);

}
