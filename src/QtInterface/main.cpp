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
** Copyright (C) Peter Dolbey 2006-7. All rights reserved.
**
****************************************************************************/
/*!
\file	main.cpp
\brief	Top leve application
*/

#include <iostream>
#include <fstream>
using namespace std;
#include <QtGui/QtGui>
#include <QtGui/QPlastiqueStyle>

#include "qoccapplication.h"
#include "Ark3DWindow.h"

int main(int argc, char *argv[])
{

	QoccApplication app( argc, argv );
	Ark3DWindow *window = new Ark3DWindow();

    window->show();

	int retval = app.exec();
	return retval;
}
