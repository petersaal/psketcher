#include "TangentEdge2D.h"

using namespace std;
using namespace GiNaC;

TangentEdge2D::TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2):
edge1_(edge1),
edge2_(edge2)
{
	AddPrimitive(edge1);
	AddPrimitive(edge2);

	ex s_1, t_1, s_2, t_2;	// tangent vector for edge1 (s_1,t_1) and tangent vector for edge2 (s_2,t_2)

	if(point_num_1 == Point1)
	{	// use point1 of edge1
		edge1->GetTangent1(s_1,t_1);
	} else {
		// use point2 of edge1
		edge1->GetTangent2(s_1,t_1);
	}

	if(point_num_2 == Point1)
	{
		// use point1 of edge2
		edge2->GetTangent1(s_2,t_2);
	} else {
		// use point2 of edge2
		edge2->GetTangent2(s_2,t_2);
	}

	// create the expression object that will store the constraint
	boost::shared_ptr<ex> new_constraint(new ex);

	// Calculate the dot product between the two tangent vectors
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	*new_constraint = pow((s_1*s_2 + t_1*t_2),2)-1;

	// populate the lists
	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
