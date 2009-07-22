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
** Copyright (C) 2006-2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#include <string>
#include <sstream>
#include <iostream>

#include "DOF.h"

using namespace std;

// Initialize private static variables for DOF and PrimitiveBase classes
unsigned DOF::next_id_number_ = 1;

DOF::DOF (bool free, bool dependent) :
id_number_(next_id_number_++),free_(free), dependent_(dependent),
database_(0),
delete_me_(false)
{
	// by default, name variable using id_number_
	stringstream variable_name;
	variable_name << "dof" << id_number_;

	name_ = variable_name.str();
}

DOF::DOF ( const char *name, bool free, bool dependent) :
id_number_(next_id_number_++),free_(free), dependent_(dependent),
database_(0),
delete_me_(false)
{
	name_ = name;
}

DOF::DOF (unsigned id, bool dependent) :
id_number_(id),free_(false), dependent_(dependent),
database_(0),
delete_me_(false)
{

}