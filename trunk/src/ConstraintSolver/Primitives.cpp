#include "Primitives.h"

using namespace std;
using namespace GiNaC;

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


Point :: Point ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new DOF(x,x_free)),
y_(new DOF(y,y_free)),
z_(new DOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}

Point2D :: Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free):
s_(new DOF(s,s_free)),
t_(new DOF(t,t_free)),
Primitive2DBase(sketch_plane)
{
	dof_list_.push_back(s_);
	dof_list_.push_back(t_);
}

Line :: Line(const PointPointer point1, const PointPointer point2)
{
	x1_ = point1->GetXDOF();
	y1_ = point1->GetYDOF();
	z1_ = point1->GetZDOF();

	x2_ = point2->GetXDOF();
	y2_ = point2->GetYDOF();
	z2_ = point2->GetZDOF();

	dof_list_.push_back(x1_);
	dof_list_.push_back(y1_);
	dof_list_.push_back(z1_);

	dof_list_.push_back(x2_);
	dof_list_.push_back(y2_);
	dof_list_.push_back(z2_);
}

Line2D :: Line2D(const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane):
Primitive2DBase(sketch_plane)
{
	s1_ = point1->GetSDOF();
	t1_ = point1->GetTDOF();

	s2_ = point2->GetSDOF();
	t2_ = point2->GetTDOF();

	dof_list_.push_back(s1_);
	dof_list_.push_back(t1_);

	dof_list_.push_back(s2_);
	dof_list_.push_back(t2_);
}

// returns global coordinates of line end points
void Line2D :: Get3DLocations(double & x1, double & y1, double & z1,
															double & x2, double & y2, double & z2)
{
	sketch_plane_->Get3DLocation(s1_->GetValue(), t1_->GetValue(), x1, y1, z1);
	sketch_plane_->Get3DLocation(s2_->GetValue(), t2_->GetValue(), x2, y2, z2);
}


// Create a constraint that defines the distance between two points
DistanceConstraintPointPoint::DistanceConstraintPointPoint(const PointPointer point1, const PointPointer point2, double distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new DOF(distance,false));
	
	dof_list_.push_back(new_dof);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1->GetXDOF()->GetVariable() - point2->GetXDOF()->GetVariable(),2) +
													pow(point1->GetYDOF()->GetVariable() - point2->GetYDOF()->GetVariable(),2) +
													pow(point1->GetZDOF()->GetVariable() - point2->GetZDOF()->GetVariable(),2))
										- new_dof->GetVariable();

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}

// Create a constraint that defines the distance between two points confined to a sketch plane
DistanceConstraintPoint2DPoint2D::DistanceConstraintPoint2DPoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new DOF(distance,false));
	
	dof_list_.push_back(new_dof);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1->GetSDOF()->GetVariable() - point2->GetSDOF()->GetVariable(),2) +
													pow(point1->GetTDOF()->GetVariable() - point2->GetTDOF()->GetVariable(),2))
										- new_dof->GetVariable();

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}

// Create a parallelism constrain between two lines
ParallelConstraintLineLine::ParallelConstraintLineLine(const LinePointer line1, const LinePointer line2)
{
	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_dx = line1->GetX1()->GetVariable() - line1->GetX2()->GetVariable();
	ex line1_dy = line1->GetY1()->GetVariable() - line1->GetY2()->GetVariable();
	ex line1_dz = line1->GetZ1()->GetVariable() - line1->GetZ2()->GetVariable();
	ex line1_length = sqrt(pow(line1_dx,2)+pow(line1_dy,2)+pow(line1_dz,2));

	ex line2_dx = line2->GetX1()->GetVariable() - line2->GetX2()->GetVariable();
	ex line2_dy = line2->GetY1()->GetVariable() - line2->GetY2()->GetVariable();
	ex line2_dz = line2->GetZ1()->GetVariable() - line2->GetZ2()->GetVariable();
	ex line2_length = sqrt(pow(line2_dx,2)+pow(line2_dy,2)+pow(line2_dz,2));

	// Calculate the dot product normalized by the vector lengths and subtract one
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	// @todo find a better function besides pow to use for the parallel constraint
	*new_constraint = pow((1/(line1_length*line2_length))*(line1_dx*line2_dx + line1_dy*line2_dy + line1_dz*line2_dz),2)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}


// Create a parallelism constrain between two lines
ParallelConstraintLine2DLine2D::ParallelConstraintLine2DLine2D(const Line2DPointer line1, const Line2DPointer line2):
line1_(line1),
line2_(line2)
{
	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_ds = line1->GetS1()->GetVariable() - line1->GetS2()->GetVariable();
	ex line1_dt = line1->GetT1()->GetVariable() - line1->GetT2()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS1()->GetVariable() - line2->GetS2()->GetVariable();
	ex line2_dt = line2->GetT1()->GetVariable() - line2->GetT2()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	// Calculate the dot product normalized by the vector lengths and subtract one
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	// @todo find a better function besides pow to use for the parallel constraint
	*new_constraint = pow((1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt),2)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}


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
		dof_list_.push_back(current_dof_list[current_dof]);
	}

	current_dof_list = up_->GetDOFList();
	for(unsigned int current_dof = 0; current_dof < current_dof_list.size(); current_dof++)
	{
		dof_list_.push_back(current_dof_list[current_dof]);
	}

	current_dof_list = base_->GetDOFList();
	for(unsigned int current_dof = 0; current_dof < current_dof_list.size(); current_dof++)
	{
		dof_list_.push_back(current_dof_list[current_dof]);
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

// Return the global coordinates for a Point2D object
void Point2D::Get3DLocation(double & x_location,double & y_location, double & z_location)const
{
	sketch_plane_->Get3DLocation(s_->GetValue(), t_->GetValue(), x_location, y_location, z_location);
}

// Return mmaMatrix vector of current x,y,z values
// used for numerical calculations only, loses associativity with DOF's
mmcMatrix Vector::GetmmcMatrix()
{
	mmcMatrix result(3,1);

	result(0,0) = x_->GetValue();
	result(1,0) = y_->GetValue();
	result(2,0) = z_->GetValue();

	return result;
}

// Return mmaMatrix vector of current x,y,z values
// used for numerical calculations only, loses associativity with DOF's
mmcMatrix Point::GetmmcMatrix()
{
	mmcMatrix result(3,1);

	result(0,0) = x_->GetValue();
	result(1,0) = y_->GetValue();
	result(2,0) = z_->GetValue();

	return result;
}

Vector :: Vector ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new DOF(x,x_free)),
y_(new DOF(y,y_free)),
z_(new DOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}

ParallelConstraint::ParallelConstraint(const Line2DPointer line1, const Line2DPointer line2)
{
	ConstraintEquationBasePointer new_constraint(new ParallelConstraintLine2DLine2D(line1,line2));
	
	SetConstraintEquation(new_constraint);
}

ParallelConstraint::ParallelConstraint(const LinePointer line1, const LinePointer line2)
{
	ConstraintEquationBasePointer new_constraint(new ParallelConstraintLineLine(line1,line2));
	
	SetConstraintEquation(new_constraint);
}

DistanceConstraint::DistanceConstraint(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	ConstraintEquationBasePointer new_constraint(new DistanceConstraintPoint2DPoint2D(point1,point2,distance));
	
	SetConstraintEquation(new_constraint);
}

DistanceConstraint::DistanceConstraint(const PointPointer point1, const PointPointer point2, double distance)
{
	ConstraintEquationBasePointer new_constraint(new DistanceConstraintPointPoint(point1,point2,distance));
	
	SetConstraintEquation(new_constraint);
}

