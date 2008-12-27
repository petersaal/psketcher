#include <string>
#include <iostream>

#include "DOF.h"

using namespace std;
using namespace GiNaC;

// Initialize private static variables for DOF and PrimitiveBase classes
unsigned DOF::next_id_number_ = 1;

DOF::DOF (bool free, bool dependent) :
id_number_(next_id_number_++),free_(free), dependent_(dependent),
database_(0)
{
	// by default, name variable using id_number_
	string variable_name;
	variable_name = "dof" + id_number_;

	variable_.set_name(variable_name);
}

DOF::DOF ( const char *name, bool free, bool dependent) :
id_number_(next_id_number_++),free_(free), dependent_(dependent),
database_(0)
{
	variable_.set_name(name);
}
