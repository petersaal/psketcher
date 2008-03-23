#ifndef PrimitivesH
#define PrimitivesH

#include <math.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <ginac/ginac.h>

#include "../mmcMatrix/mmcMatrix.h"

//Exception class
class PrimitiveException{};

// Selection masks
enum SelectionMask {None, All, Points, Edges, Constraints, Lines, Arcs};

// Abstract DOF base class
class DOF
{
	public:
		DOF (bool free, bool dependent);
		DOF ( const char *name, bool free, bool dependent);

		virtual ~DOF() {;}

		//Accessor methods
		virtual void SetValue ( double value ) = 0;
		virtual double GetValue()const = 0;
		virtual GiNaC::ex GetExpression()const {return variable_;}

		unsigned GetID()const {return id_number_;}

		const GiNaC::symbol & GetVariable()const {return variable_ ;}
		bool IsFree()const {return free_;}
		virtual void SetFree(bool free) {free_ = free;}

		bool IsDependent()const {return dependent_;}

	private:
		GiNaC::symbol variable_;
		bool free_;
		bool dependent_;
		
		// each instance of this class has a unique ID number
		unsigned id_number_;

		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;
};
typedef boost::shared_ptr<DOF> DOFPointer;

// IndependentDOF class
class IndependentDOF : public DOF
{
	public:
		IndependentDOF ( double value = 0.0, bool free = false );
		IndependentDOF ( const char *name, double value = 0.0, bool free = false );

		//Accessor methods
		void SetValue ( double value ) {value_ = value;}
		double GetValue()const {return value_;}

	private:
		double value_;
};
typedef boost::shared_ptr<IndependentDOF> IndependentDOFPointer;

// DependentDOF class
class DependentDOF : public DOF
{
	public:
		DependentDOF ( GiNaC::ex expression, std::vector<DOFPointer> source_dof_list);
		DependentDOF ( const char *name, GiNaC::ex expression, std::vector<DOFPointer> source_dof_list);

		~DependentDOF () {source_dof_list_.clear();}
		
		//Accessor methods
		void SetValue ( double value ) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		void SetFree(bool free) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		double GetValue()const;
		GiNaC::ex GetExpression()const;

	private:
		GiNaC::ex expression_;
		std::vector<DOFPointer> source_dof_list_;
};
typedef boost::shared_ptr<DependentDOF> DependentDOFPointer;

/* Now will define the merit function derived class used in the template matching */
class PrimitiveBase
{
	public:
		PrimitiveBase();
		virtual ~PrimitiveBase() {dof_list_.clear();}
		
		// Accessor methods
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}
		
		unsigned GetID()const {return id_number_;}

		// selection methods
		virtual bool IsSelected() { return selected_;}
		virtual void SetSelectable(bool selectable);
		virtual void ApplySelectionMask(SelectionMask mask);

		// display methods
		virtual void Display() {;}
		virtual void UpdateDisplay() {;}

	protected:
		std::vector<DOFPointer> dof_list_;

		bool selected_;
		bool selectable_;

	private:
		// each instance of this class has a unique ID number
		unsigned id_number_;

		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;
};
typedef boost::shared_ptr<PrimitiveBase> PrimitiveBasePointer;


// point class
class Point : public PrimitiveBase
{
	public:
		Point ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Point ( DOFPointer x, DOFPointer y, DOFPointer z );

		void ApplySelectionMask(SelectionMask mask);

		DOFPointer GetXDOF()const {return x_;}
		DOFPointer GetYDOF()const {return y_;}
		DOFPointer GetZDOF()const {return z_;}

		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Point> PointPointer;

// vector class
class Vector : public PrimitiveBase
{
	public:
		Vector ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

		void ApplySelectionMask(SelectionMask mask);

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Vector> VectorPointer;

// sketch plane class (includes up vector)
class SketchPlane : public PrimitiveBase
{
	public:
		SketchPlane ( VectorPointer normal, VectorPointer up, PointPointer base);
		void Get3DLocation ( double s, double t, double & x, double & y, double & z);
		double GetSTLocation( double x, double y, double z, double &s, double &t);
		void GetABCD ( double & coef_a, double & coef_b, double & coef_c, double & coef_d);
		
		void ApplySelectionMask(SelectionMask mask);

		VectorPointer GetNormal() {return normal_;}
		VectorPointer GetUp() {return up_;}
		PointPointer GetBase() {return base_;}
	private:
		VectorPointer normal_;
		VectorPointer up_;
		PointPointer base_;
};
typedef boost::shared_ptr<SketchPlane> SketchPlanePointer;

// Base class for points constrained to a sketch plane
class Primitive2DBase : public PrimitiveBase
{
	public:
		Primitive2DBase(SketchPlanePointer sketch_plane) {sketch_plane_ = sketch_plane;}
		SketchPlanePointer GetSketchPlane() { return sketch_plane_;}

	protected:
		SketchPlanePointer sketch_plane_;
};
typedef boost::shared_ptr<Primitive2DBase> Primitive2DBasePointer;

// Point2D class (a point constrained to a sketch plane)
class Point2D : public Primitive2DBase
{
	public:
		void ApplySelectionMask(SelectionMask mask);

		Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		DOFPointer GetSDOF()const {return s_;}
		DOFPointer GetTDOF()const {return t_;}
		double GetSValue()const {return s_->GetValue();}
		double GetTValue()const {return t_->GetValue();}
		bool SIsFree() const {return s_->IsFree();}
		bool TIsFree() const {return t_->IsFree();}

		void SetSValue(double value){s_->SetValue(value);}
		void SetTValue(double value) {t_->SetValue(value);}
		void SetSFree(bool free) {s_->SetFree(free);}
		void SetTFree(bool free) {t_->SetFree(free);}

		void Get3DLocation(double & x_location, double & y_location, double & z_location)const;

	private:
		DOFPointer s_;
		DOFPointer t_;
};
typedef boost::shared_ptr<Point2D> Point2DPointer;

// Abstract base class for 2D edges
enum EdgePointNumber {Point1, Point2};
class Edge2DBase : public Primitive2DBase
{
	public:
		Edge2DBase(SketchPlanePointer sketch_plane);
		virtual ~Edge2DBase() {;}
		
		virtual void ApplySelectionMask(SelectionMask mask);

		// virtual methods that must be implemented by child classes
		virtual Point2DPointer GetPoint1() = 0;		// returns end point of edge (these may include dependentDOF's as is the case of the arc primitive)
		virtual Point2DPointer GetPoint2() = 0;
		virtual void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component) = 0;  // returns expression that defines tangent vector for each endpoint of the edge
		virtual void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component) = 0;

		// methods implemented by this base class
		bool IsPointCoincident(EdgePointNumber my_point_number, boost::shared_ptr<Edge2DBase> other_edge, EdgePointNumber other_point_number);
	protected:
};
typedef boost::shared_ptr<Edge2DBase> Edge2DBasePointer;

// line class
class Line : public PrimitiveBase
{
	public:
		Line (const PointPointer point1, const PointPointer point2);

		DOFPointer GetX1()const {return x1_;}
		DOFPointer GetY1()const {return y1_;}
		DOFPointer GetZ1()const {return z1_;}
		DOFPointer GetX2()const {return x2_;}
		DOFPointer GetY2()const {return y2_;}
		DOFPointer GetZ2()const {return z2_;}

		virtual void ApplySelectionMask(SelectionMask mask);

	private:
		DOFPointer x1_;
		DOFPointer y1_;
		DOFPointer z1_;
		DOFPointer x2_;
		DOFPointer y2_;
		DOFPointer z2_;
};
typedef boost::shared_ptr<Line> LinePointer;

// Line2D class
class Line2D : public Edge2DBase
{
	public:
		Line2D (const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		DOFPointer GetS1()const {return s1_;}
		DOFPointer GetT1()const {return t1_;}

		DOFPointer GetS2()const {return s2_;}
		DOFPointer GetT2()const {return t2_;}

		void Get3DLocations(double & x1, double & y1, double & z1,
												double & x2, double & y2, double & z2);

		Point2DPointer GetPoint1();
		Point2DPointer GetPoint2();

		void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component);  // returns expression that defines tangent vector for each endpoint of the edge
		void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component);

		void ApplySelectionMask(SelectionMask mask);

	private:
		DOFPointer s1_;
		DOFPointer t1_;

		DOFPointer s2_;
		DOFPointer t2_;
};
typedef boost::shared_ptr<Line2D> Line2DPointer;


// constraint equation class
class ConstraintEquationBase : public PrimitiveBase
{
	public:
		virtual ~ConstraintEquationBase() {constraints_.clear();}
	
		// Accessor methods
		const std::vector< boost::shared_ptr<GiNaC::ex> > & GetConstraintList() {return constraints_;}
		const std::vector<double> & GetWeightList() {return weight_list_;}

		virtual void ApplySelectionMask(SelectionMask mask);

	protected:
		// constraints and weight_list_ are parallel vectors
		// stores constraints
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_; 

		// this vector stores the weights for the constraints
		std::vector< double > weight_list_;
};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;

class DistancePoint2D : public ConstraintEquationBase
{
	public:
		DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		double GetActualDistance();
		void SetValue(double value) {distance_->SetValue(value);}

	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;

class DistancePoint : public ConstraintEquationBase
{
	public:
		DistancePoint(const PointPointer point1, const PointPointer point2, double distance);

	private:
};
typedef boost::shared_ptr<DistancePoint> DistancePointPointer;

class ParallelLine : public ConstraintEquationBase
{
	public:
		ParallelLine(const LinePointer line1, const LinePointer line2);

	private:

};
typedef boost::shared_ptr<ParallelLine> ParallelLinePointer;

class ParallelLine2D : public ConstraintEquationBase
{
	public:
		ParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);

	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

};
typedef boost::shared_ptr<ParallelLine2D> ParallelLine2DPointer;

class TangentEdge2D : public ConstraintEquationBase
{
	public:
		TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

	protected:
		Edge2DBasePointer edge1_;
		Edge2DBasePointer edge2_;
};
typedef boost::shared_ptr<TangentEdge2D> TangentEdge2DPointer;

class AngleLine2D : public ConstraintEquationBase
{
	public:
		AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */);

	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

		DOFPointer angle_;
};
typedef boost::shared_ptr<AngleLine2D> AngleLine2DPointer;

// Line2D class
class Arc2D : public Edge2DBase
{
	public:
		Arc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, bool radius_free = false);
		Arc2D (DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane);

		DOFPointer GetSCenter()const {return s_center_;}
		DOFPointer GetTCenter()const {return t_center_;}

		DOFPointer GetTheta1()const {return theta_1_;}
		DOFPointer GetTheta2()const {return theta_2_;}

		DOFPointer GetRadius()const {return radius_;}

		void Get3DLocations(double & x_center, double & y_center, double & z_center);

		Point2DPointer GetPoint1(){return point1_;}
		Point2DPointer GetPoint2(){return point2_;}

		Point2DPointer GeneratePoint1();
		Point2DPointer GeneratePoint2();

		void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component);  // returns expression that defines tangent vector for each endpoint of the edge
		void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component);

		void ApplySelectionMask(SelectionMask mask);
	
	protected:
		// parameters that define the arc
		DOFPointer s_center_;
		DOFPointer t_center_;

		DOFPointer theta_1_;	// start angle
		DOFPointer theta_2_;	// end angle

		DOFPointer radius_;

		// Points based on dependent DOF's that provide the location of the arc endpoints
		// It is important that these points be maintained here so that Edge2DCBase point coincident checks can be performed by comparing DOF pointers
		// If these points are generated for each call to GetPoint1() then, for every call to GetPoint*(), the pointers to the S and T DOF's will be unique
		Point2DPointer point1_;
		Point2DPointer point2_;
};
typedef boost::shared_ptr<Arc2D> Arc2DPointer;

// 2D edge loop classs
class EdgeLoop2D
{
	public:
		// constructor and destructor
		EdgeLoop2D() {;}
		EdgeLoop2D(std::vector<Edge2DBasePointer> edge_list);
		~EdgeLoop2D() {edge_list_.clear();}

		// Acessor methods
		void AddEdge(Edge2DBasePointer new_edge);
		
		// utility methods
		bool IsLoopValid();		// Check to insure that the loop is well defined

	private:
		std::vector<Edge2DBasePointer> edge_list_;
};
typedef boost::shared_ptr<EdgeLoop2D> EdgeLoop2DPointer;

#endif //PrimitivesH
