%module ark3d;
%{
#include "../ConstraintSolver/Primitives.h"
#include "../ConstraintSolver/Ark3DModel.h"
%}
#extern Ark3DModel ark3d_model_;
%include "../ConstraintSolver/Ark3DModel.h"
%include "../ConstraintSolver/Primitives.h"


