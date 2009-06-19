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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "Ark3DWidget.h"

class QAction;
class QLabel;
class QMenu;

class Ark3DWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ark3DWindow();

private slots:
    void about();
	void triggerSketchActionGroup();
	void switchToSelectionMode();

private:
    void createActions();
    void createMenus();
	void createToolBars();

    QMenu *fileMenu;
    QMenu *editMenu;
	QMenu *viewMenu;
    QMenu *helpMenu;

	QToolBar *viewToolBar;

    QAction *exitAction;
    QAction *aboutAction;

	QAction *fitAction;
	QAction *zoomAction;

	QAction *viewShadedAction;
	QAction *viewWireframeAction;
	QAction *viewNoHiddenLineAction;

	// Debugging actions
	QAction *generateSketchAction;
	QAction *solveConstraintsAction;
	QToolBar *debugToolBar;

	// File actions
	QAction *saveAction;
	QAction *openAction;
	QAction *saveAsAction;
	QAction *newFileAction;
	QAction *exportDXFAction;

	// Edit actions
	QAction *undoAction;
	QAction *redoAction;

	// Sketching action
	QActionGroup *sketchActionGroup;
    QAction *selectAction;
	QAction *makeLineAction;
	QAction *makePointAction;
	QAction *makeArcAction;
	QAction *makeDistanceConstraintAction;
	QAction *makeAngleConstraintAction;
	QAction *makeParallelConstraintAction;
	QAction *makeTangentConstraintAction;
	QToolBar *sketchToolBar;

	// The OpenCASCADE Qt widget and the QGraphicsScene;
	Ark3DWidget		*view;
	QGraphicsScene 	*scene;

	QString myLastFolder;
};

#endif

