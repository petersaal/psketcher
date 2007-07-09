#ifndef PrimitivesH
#define PrimitivesH


#include <vector>
#include <boost/shared_ptr.hpp>
#include <ginac/ginac.h>

#include "../mmcMatrix/mmcMatrix.h"

// dof class
class DOF
{
	public:
		DOF ( double value = 0.0, bool free = false );
		DOF ( const char *name, double value = 0.0, bool free = false );

		//Accessor methods
		void SetValue ( double value ) {value_ = value;}
		double GetValue()const {return value_;}

		const GiNaC::symbol & GetVariable()const {return variable_ ;}
		bool IsFree()const {return free_;}

	private:
		GiNaC::symbol variable_;
		bool free_;
		double value_;
		int reference_counter_;
};

typedef boost::shared_ptr<DOF> DOFPointer;


/* Now will define the merit function derived class used in the template matching */
class PrimitiveBase
{
	public:
		virtual ~PrimitiveBase() {dof_list_.clear();}
		
		// Accessor methods
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}

	protected:
		std::vector<DOFPointer> dof_list_;
};

typedef boost::shared_ptr<PrimitiveBase> PrimitiveBasePointer;


// point class
class Point : public PrimitiveBase
{
	public:
		Point ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		Point ( DOFPointer x, DOFPointer y, DOFPointer z );

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
		Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

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

	private:
		VectorPointer normal_;
		VectorPointer up_;
		PointPointer base_;
};
typedef boost::shared_ptr<SketchPlane> SketchPlanePointer;

// Point2D class (a point constrained to a sketch plane)
class Point2D : public PrimitiveBase
{
	public:
		Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		DOFPointer GetSDOF()const {return s_;}
		DOFPointer GetTDOF()const {return t_;}

		void Get3DLocation(double & x_location, double & y_location, double & z_location)const;

	private:
		DOFPointer s_;
		DOFPointer t_;
		SketchPlanePointer sketch_plane_;
};
typedef boost::shared_ptr<Point2D> Point2DPointer;

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
class Line2D : public PrimitiveBase
{
	public:
		Line2D (const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		DOFPointer GetS1()const {return s1_;}
		DOFPointer GetT1()const {return t1_;}

		DOFPointer GetS2()const {return s2_;}
		DOFPointer GetT2()const {return t2_;}

		void Get3DLocations(double & x1, double & y1, double & z1,
												double & x2, double & y2, double & z2);

	private:
		DOFPointer s1_;
		DOFPointer t1_;

		DOFPointer s2_;
		DOFPointer t2_;

		SketchPlanePointer sketch_plane_;
};
typedef boost::shared_ptr<Line2D> Line2DPointer;

// constraint equation class
class ConstraintEquationBase
{
	public:
		virtual ~ConstraintEquationBase() {constraints_.clear(); dof_list_.clear();}
	
		// Accessor methods
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}
		const std::vector< boost::shared_ptr<GiNaC::ex> > & GetConstraintList() {return constraints_;}
		const std::vector<double> & GetWeightList() {return weight_list_;}

	protected:
		// constraints and weight_list_ are parallel vectors
		// stores constraints
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_; 

		// this vector stores the weights for the constraints
		std::vector< double > weight_list_; 

		std::vector<DOFPointer> dof_list_;

};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;

class DistanceConstraint : public ConstraintEquationBase
{
	public:
		DistanceConstraint(const PointPointer point1, const PointPointer point2, double distance);
		DistanceConstraint(const Point2DPointer point1, const Point2DPointer point2, double distance);

	private:
};
typedef boost::shared_ptr<DistanceConstraint> DistanceConstraintPointer;

class ParallelConstraint : public ConstraintEquationBase
{
	public:
		ParallelConstraint(const LinePointer line1, const LinePointer line2);
		ParallelConstraint(const Line2DPointer line1, const Line2DPointer line2);

	private:

};
typedef boost::shared_ptr<ParallelConstraint> ParallelConstraintPointer;



#endif //PrimitivesH
