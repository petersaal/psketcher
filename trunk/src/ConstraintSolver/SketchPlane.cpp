#include "SketchPlane.h"

using namespace std;
using namespace GiNaC;

// Constructor for SketchPlane class
SketchPlane::SketchPlane ( VectorPointer normal, VectorPointer up, PointPointer base):
normal_(normal),
up_(up),
base_(base)
{
	// @TODO need to check to insure that the normal vector and the up vector are normalized, calculations later on will assume this

	// @TODO need to make sure that up vector lies in plane defined by normal vector (up vector must be orthogonal to normal vector)

	// Populate the primitve base classes DOF list for each of the primitives referenced by this SketchPlane
	std::vector<DOFPointer> current_dof_list = normal_->GetDOFList();
	for(unsigned int current_dof = 0; current_dof < current_dof_list.size(); current_dof++)
	{
		AddDOF(current_dof_list[current_dof]);
	}

	current_dof_list = up_->GetDOFList();
	for(unsigned int current_dof = 0; current_dof < current_dof_list.size(); current_dof++)
	{
		AddDOF(current_dof_list[current_dof]);
	}

	current_dof_list = base_->GetDOFList();
	for(unsigned int current_dof = 0; current_dof < current_dof_list.size(); current_dof++)
	{
		AddDOF(current_dof_list[current_dof]);
	}
}

void SketchPlane::GetABCD ( double & coef_a, double & coef_b, double & coef_c, double & coef_d)
{
	mmcMatrix normal_vector = normal_->GetmmcMatrix();
	mmcMatrix base_vector = base_->GetmmcMatrix();

	coef_a = normal_vector(0,0);
	coef_b = normal_vector(1,0);
	coef_c = normal_vector(2,0);
	coef_d = normal_vector.DotProduct(base_vector);
}

// Return the global coordinates of a point on the sketch plane
void SketchPlane::Get3DLocation ( double s, double t, double & x, double & y, double & z)
{
	mmcMatrix j_vector = up_->GetmmcMatrix();  // t axis direction vector in sketch plane
	mmcMatrix i_vector = j_vector.CrossProduct(normal_->GetmmcMatrix()); // s axis direction vector in sketch plane

	mmcMatrix global_position = base_->GetmmcMatrix() + s*i_vector + t*j_vector;

	x = global_position(0,0);
	y = global_position(1,0);
	z = global_position(2,0);
}

// Find the closest pont on the sketch plane to the global coordinates x,y, and z and return its s and t coordinates
// the return value is the distance of the global point to the sketch plane (if the sketch plane normal vector is not normalized than the distance will be scaled)
double SketchPlane::GetSTLocation( double x, double y, double z, double &s, double &t)
{
	mmcMatrix global_point(3,1);
	global_point(0,0) = x;
	global_point(1,0) = y;
	global_point(2,0) = z;

	mmcMatrix normal_vector = normal_->GetmmcMatrix();
	mmcMatrix j_vector = up_->GetmmcMatrix();  // t axis direction vector in sketch plane
	mmcMatrix i_vector = j_vector.CrossProduct(normal_vector); // s axis direction vector in sketch plane

	mmcMatrix a_matrix = i_vector;
	a_matrix = a_matrix.CombineAsRow(j_vector);
	a_matrix = a_matrix.CombineAsRow(normal_vector);

	mmcMatrix result = a_matrix.GetInverse()*(global_point - base_->GetmmcMatrix());

	s = result(0,0);
	t = result(1,0);

	return result(2,0);
}

void SketchPlane::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All)
		SetSelectable(true);
	else
		SetSelectable(false);
}