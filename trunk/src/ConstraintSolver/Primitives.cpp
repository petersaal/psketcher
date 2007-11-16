#include "Primitives.h"

using namespace std;
using namespace GiNaC;

IndependentDOF ::IndependentDOF ( double value, bool free):
DOF(free,false /*dependent*/)
{
	value_=value;
}

IndependentDOF :: IndependentDOF ( const char *name, double value, bool free):
DOF(name,free,false /*dependent*/)
{
	value_ = value;
}

DependentDOF :: DependentDOF (ex expression, std::vector<DOFPointer> source_dof_list):
DOF(false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

DependentDOF :: DependentDOF ( const char *name, ex expression, std::vector<DOFPointer> source_dof_list):
DOF(name,false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

double DependentDOF::GetValue()const
{
	// For each independent DOF in the list, substitute its value into the expression that defines this dependent DOF
	GiNaC::ex current_expression = GetExpression();
	double result;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(!source_dof_list_[current_dof]->IsDependent())
		{
			current_expression = current_expression.subs(source_dof_list_[current_dof]->GetVariable() ==
														 source_dof_list_[current_dof]->GetValue(),subs_options::no_pattern);
		}
	}

	// Now evaluate the expression to a numeric value
	// check to make sure the expression evaluates to a numeric value
	if (is_a<numeric>(current_expression)) {
		result = ex_to<numeric>(current_expression).to_double();
	} else {
		throw PrimitiveException();
  	}

	return result;
}

GiNaC::ex DependentDOF::GetExpression()const
{
	// For each dependent DOF in the list, substitute its expression into the overall expression that defines this dependent DOF
	GiNaC::ex result = expression_;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(source_dof_list_[current_dof]->IsDependent())
		{
			result = result.subs(source_dof_list_[current_dof]->GetVariable() == source_dof_list_[current_dof]->GetExpression(),subs_options::no_pattern);
		}
	}

	return result;
}

Point :: Point ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new IndependentDOF(x,x_free)),
y_(new IndependentDOF(y,y_free)),
z_(new IndependentDOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}

Point2D :: Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free):
s_(new IndependentDOF(s,s_free)),
t_(new IndependentDOF(t,t_free)),
Primitive2DBase(sketch_plane)
{
	dof_list_.push_back(s_);
	dof_list_.push_back(t_);
}

Point2D :: Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane):
s_(s),
t_(t),
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
Edge2DBase(sketch_plane)
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

Point2DPointer Line2D::GetPoint1()
{
	// create the actual point object
	Point2DPointer result(new Point2D(GetS1(), GetT1(), GetSketchPlane()));
	return result;
}

Point2DPointer Line2D::GetPoint2()
{
	// create the actual point object
	Point2DPointer result(new Point2D(GetS2(), GetT2(), GetSketchPlane()));
	return result;
}

void Line2D::GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	ex length = sqrt(pow(GetS1()->GetVariable()-GetS2()->GetVariable(),2)+pow(GetT1()->GetVariable()-GetT2()->GetVariable(),2));
	
	s_component = (GetS1()->GetVariable() - GetS2()->GetVariable())/length;
	t_component = (GetT1()->GetVariable() - GetT2()->GetVariable())/length;
}

void Line2D::GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	ex length = sqrt(pow(GetS1()->GetVariable()-GetS2()->GetVariable(),2)+pow(GetT1()->GetVariable()-GetT2()->GetVariable(),2));
	
	s_component = (GetS2()->GetVariable() - GetS1()->GetVariable())/length;
	t_component = (GetT2()->GetVariable() - GetT1()->GetVariable())/length;
}

// returns global coordinates of line end points
void Line2D :: Get3DLocations(double & x1, double & y1, double & z1,
															double & x2, double & y2, double & z2)
{
	sketch_plane_->Get3DLocation(s1_->GetValue(), t1_->GetValue(), x1, y1, z1);
	sketch_plane_->Get3DLocation(s2_->GetValue(), t2_->GetValue(), x2, y2, z2);
}


// Create a constraint that defines the distance between two points
DistancePoint::DistancePoint(const PointPointer point1, const PointPointer point2, double distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	
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
DistancePoint2D::DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance):
point1_(point1),
point2_(point2),
distance_(distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	
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
ParallelLine::ParallelLine(const LinePointer line1, const LinePointer line2)
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
ParallelLine2D::ParallelLine2D(const Line2DPointer line1, const Line2DPointer line2):
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
	*new_constraint = pow((1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt),2)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}

TangentEdge2D::TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2):
edge1_(edge1),
edge2_(edge2)
{

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
x_(new IndependentDOF(x,x_free)),
y_(new IndependentDOF(y,y_free)),
z_(new IndependentDOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}



// Create an angle constraint between two lines
AngleLine2D::AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */):
line1_(line1),
line2_(line2),
angle_(angle)
{
	// Create a DOF for the angle parameter
	DOFPointer new_dof(new IndependentDOF(angle_,false));
	
	dof_list_.push_back(new_dof);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_ds = line1->GetS1()->GetVariable() - line1->GetS2()->GetVariable();
	ex line1_dt = line1->GetT1()->GetVariable() - line1->GetT2()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS1()->GetVariable() - line2->GetS2()->GetVariable();
	ex line2_dt = line2->GetT1()->GetVariable() - line2->GetT2()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	ex desired_angle = new_dof->GetVariable();

	// Calculate the dot product normalized by the vector lengths and subtract the cos of the desired angle
	// this expression will be zero when the lines are at the desired angle
	*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(mmcPI-desired_angle);

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}


// create an arc
Arc2D::Arc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
			  bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free):
s_center_(new IndependentDOF(s_center,s_center_free)),
t_center_(new IndependentDOF(t_center,t_center_free)),
theta_1_(new IndependentDOF(theta_1,theta_1_free)),
theta_2_(new IndependentDOF(theta_2,theta_2_free)),
radius_(new IndependentDOF(radius,radius_free)),
Edge2DBase(sketch_plane)
{
	dof_list_.push_back(s_center_);
	dof_list_.push_back(t_center_);
	dof_list_.push_back(theta_1_);
	dof_list_.push_back(theta_2_);
	dof_list_.push_back(radius_);

	point1_ = GeneratePoint1();
	point2_ = GeneratePoint2();
}

Arc2D::Arc2D (DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane):
s_center_(s_center),
t_center_(t_center),
theta_1_(theta_1),
theta_2_(theta_2),
radius_(radius),
Edge2DBase(sketch_plane)
{
	dof_list_.push_back(s_center_);
	dof_list_.push_back(t_center_);
	dof_list_.push_back(theta_1_);
	dof_list_.push_back(theta_2_);
	dof_list_.push_back(radius_);

	point1_ = GeneratePoint1();
	point2_ = GeneratePoint2();
}

void Arc2D::Get3DLocations(double & x_center, double & y_center, double & z_center)
{
	sketch_plane_->Get3DLocation(s_center_->GetValue(), t_center_->GetValue(), x_center, y_center, z_center);
}

// Return a point that will follow the first endpoint of the arc
// This point will use dependent DOF's to define its location
Point2DPointer Arc2D::GeneratePoint1()
{
	// Create expressions defining s and t coordinates of the first endpoint of the arc
	ex s_1 = s_center_->GetVariable() + radius_->GetVariable()*cos(theta_1_->GetVariable());
	ex t_1 = t_center_->GetVariable() + radius_->GetVariable()*sin(theta_1_->GetVariable());

	// create DOF lists for each DOF
	std::vector <DOFPointer> s_1_dof_list;
	s_1_dof_list.push_back(s_center_);
	s_1_dof_list.push_back(radius_);
	s_1_dof_list.push_back(theta_1_);

	std::vector <DOFPointer> t_1_dof_list;
	t_1_dof_list.push_back(t_center_);
	t_1_dof_list.push_back(radius_);
	t_1_dof_list.push_back(theta_1_);

	// create dependent DOF's based on the above expressions
	DOFPointer s_dof(new DependentDOF(s_1, s_1_dof_list));
	DOFPointer t_dof(new DependentDOF(t_1, t_1_dof_list));

	// create the actual point object
	Point2DPointer result(new Point2D(s_dof, t_dof, sketch_plane_));
	return result;
}

// Return a point that will follow the second endpoint of the arc
// This point will use dependent DOF's to define its location
Point2DPointer Arc2D::GeneratePoint2()
{
	// Create expressions defining s and t coordinates of the second endpoint of the arc
	ex s_2 = s_center_->GetVariable() + radius_->GetVariable()*cos(theta_2_->GetVariable());
	ex t_2 = t_center_->GetVariable() + radius_->GetVariable()*sin(theta_2_->GetVariable());

	// create DOF lists for each DOF
	std::vector <DOFPointer> s_2_dof_list;
	s_2_dof_list.push_back(s_center_);
	s_2_dof_list.push_back(radius_);
	s_2_dof_list.push_back(theta_2_);

	std::vector <DOFPointer> t_2_dof_list;
	t_2_dof_list.push_back(t_center_);
	t_2_dof_list.push_back(radius_);
	t_2_dof_list.push_back(theta_2_);

	// create dependent DOF's based on the above expressions
	DOFPointer s_dof(new DependentDOF(s_2, s_2_dof_list));
	DOFPointer t_dof(new DependentDOF(t_2, t_2_dof_list));

	// create the actual point object
	Point2DPointer result(new Point2D(s_dof, t_dof, sketch_plane_));
	return result;
}

void Arc2D::GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	s_component = sin(GetTheta1()->GetVariable());
	t_component = -cos(GetTheta1()->GetVariable());
}

void Arc2D::GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	s_component = -sin(GetTheta2()->GetVariable());
	t_component = cos(GetTheta2()->GetVariable());
}

Edge2DBase::Edge2DBase(SketchPlanePointer sketch_plane):
Primitive2DBase(sketch_plane)
{
}

bool Edge2DBase::IsPointCoincident(EdgePointNumber my_point_number, boost::shared_ptr<Edge2DBase> other_edge, EdgePointNumber other_point_number)
{
	bool result;

	Point2DPointer my_point;
	Point2DPointer other_point;
	
	// assign the Point pointers
	if(my_point_number == Point1)
		my_point = GetPoint1();
	else
		my_point = GetPoint2();

	if(other_point_number == Point1)
		other_point = other_edge->GetPoint1();
	else
		other_point = other_edge->GetPoint2();

	// perform the comparison
	if(my_point->GetSDOF() == other_point->GetSDOF() && my_point->GetTDOF() == other_point->GetTDOF())
		result = true;
	else
		result = false;

	return result;
}

EdgeLoop2D::EdgeLoop2D(std::vector<Edge2DBasePointer> edge_list):
edge_list_(edge_list)
{
}

// method to check to see if the loop is valid
// each consecutive edge must have a coincident node and the first and last edge must have a coincident node
bool EdgeLoop2D::IsLoopValid()
{
	// check to insure that there is more than one edge
	if(edge_list_.size() < 2 )
		return false;
	
	// check to insure that there are no duplicate edges 
	for(unsigned int current_edge = 0; current_edge < edge_list_.size()-1; current_edge++)
		for(unsigned int other_edge = current_edge+1; other_edge < edge_list_.size(); other_edge++)
		{
			if(edge_list_[current_edge] == edge_list_[other_edge])
				return false;
		}

	// check to insure that each edge shares an endpoint with the next edge in the list
	for(unsigned int current_edge = 0; current_edge < edge_list_.size(); current_edge++)
	{
		unsigned int next_edge;

		if(current_edge < edge_list_.size()-1)
			next_edge = current_edge + 1;
		else
			next_edge = 0;  // this is the case of the last edge in the list, its next edge will be the first edge
		
		bool test1 = edge_list_[current_edge]->IsPointCoincident(Point1, edge_list_[next_edge],Point1);
		bool test2 = edge_list_[current_edge]->IsPointCoincident(Point1, edge_list_[next_edge],Point2);
		bool test3 = edge_list_[current_edge]->IsPointCoincident(Point2, edge_list_[next_edge],Point1);
		bool test4 = edge_list_[current_edge]->IsPointCoincident(Point2, edge_list_[next_edge],Point2);
		
		if( ! (test1 || test2 || test3 || test4))
			return false;
	}
	
	// no problems found
	return true;
}

// Add an edge to the edge loop
void EdgeLoop2D::AddEdge(Edge2DBasePointer new_edge)
{
	edge_list_.push_back(new_edge);
}





Point2DPointer CreatePoint2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free)
{
	Point2DPointer result(new Point2D(s,t,sketch_plane,s_free,t_free));
	return result;
}

PointPointer CreatePoint ( double x, double y, double z, bool x_free, bool y_free, bool z_free)
{
	PointPointer result(new Point(x,y,z,x_free,y_free,z_free));
	return result;
}

VectorPointer CreateVector ( double x, double y, double z, bool x_free, bool y_free, bool z_free)
{
	VectorPointer result(new Vector(x,y,z,x_free,y_free,z_free));
	return result;
}

SketchPlanePointer CreateSketchPlane ( VectorPointer normal, VectorPointer up, PointPointer base)
{
	SketchPlanePointer result(new SketchPlane(normal,up,base));
	return result;
}

Arc2DPointer CreateArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
               bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	Arc2DPointer result(new Arc2D(s_center, t_center, theta_1, theta_2, radius, sketch_plane,s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	return result;
}

Line2DPointer CreateLine2D (const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane)
{
	Line2DPointer result(new Line2D(point1, point2, sketch_plane));
	return result;
}

DistancePoint2DPointer CreateDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	DistancePoint2DPointer result(new DistancePoint2D(point1,point2,distance));
	return result;
}

ParallelLine2DPointer CreateParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	ParallelLine2DPointer result (new ParallelLine2D(line1, line2));
	return result;
}

AngleLine2DPointer CreateAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */)
{
	AngleLine2DPointer result(new AngleLine2D(line1,line2,angle));
	return result;
}

TangentEdge2DPointer CreateTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	TangentEdge2DPointer result(new TangentEdge2D(edge1, point_num_1, edge2, point_num_2));
	return result;
}




