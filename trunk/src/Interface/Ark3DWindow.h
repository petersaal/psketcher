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
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();
	void statusMessage (const QString aMessage);
	void triggerSketchActionGroup();

private:
    void createActions();
    void createMenus();
	void createToolBars();

    QMenu *fileMenu;
    QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *viewActionsMenu;
	QMenu *viewDisplayMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
	QMenu *gridMenu;

	QToolBar *viewToolBar;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *aboutAction;

	QAction *fitAction;
	QAction *fitAllAction;
	QAction *zoomAction;
	QAction *panAction;
	QAction *rotAction;

    QAction *viewFrontAction;
	QAction *viewBackAction;
	QAction *viewTopAction;
	QAction *viewBottomAction;
	QAction *viewLeftAction;
	QAction *viewRightAction;
	QAction *viewAxoAction;
	QAction *viewResetAction;
	QAction *viewGridAction; /** Action for grid view */

	QAction *viewShadedAction;
	QAction *viewWireframeAction;
	QAction *viewNoHiddenLineAction;

	QActionGroup *viewModeActionGroup;

	QAction *backgroundAction;

	// Debugging actions
	QAction *generateSketchAction;
	QAction *solveConstraintsAction;
	QToolBar *debugToolBar;

	// Sketching action
	QActionGroup *sketchActionGroup;
    QAction *selectAction;
	QAction *makeLineAction;
	QAction *makePointAction;
	QAction *makeDistanceConstraintAction;
	QAction *makeAngleConstraintAction;
	QToolBar *sketchToolBar;


	// The OpenCASCADE Qt widget and the QGraphicsScene;
	Ark3DWidget		*view;
	QGraphicsScene 	*scene;

	QString myLastFolder;

};

#endif

