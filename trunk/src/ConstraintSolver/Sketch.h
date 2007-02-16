#ifndef SketchH
#define SketchH

#include "ConstraintSolver.h"


/* Now will define the merit function derived class used in the template matching */
class Sketch
{
public:
	// virtual Sketch()
	// virtual ~Sketch()
	// Draw()
	// SolveConstraintSystem()
	// AddPrimitive()
	// AddConstraint()
	// DefineConnectivity()

private:
	// constraint equation list
    vector<*Primitive> primitive_list_;
    DOFList dof_list_ 
    // sketch plane (includes normal vector and up vector definition)
};

#endif //SketchH
