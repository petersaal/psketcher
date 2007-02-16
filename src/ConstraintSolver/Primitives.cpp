#include "Primitives.h"


DOF :: DOF(double value, bool free)
{
	free_ = free;
	value_ = value;

	reference_counter = 1;

	return this;
}


DOF :: DOF(const char *name, double value, bool free)
{

	free_ = free;
	variable_.set_name(name);
	value_ = value;

	reference_counter = 1;

	return this;
}









