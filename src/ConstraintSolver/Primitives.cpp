#include "Primitives.h"

using namespace std;


DOF :: DOF(double value, bool free)
{
	free_ = free;
	value_ = value;

}


DOF :: DOF(const char *name, double value, bool free)
{
	free_ = free;
	variable_.set_name(name);
	value_ = value;
}









