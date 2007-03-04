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

		virtual void Draw() const {;}
		
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

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Point> PointPointer;

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

// vector class
class Vector : public PrimitiveBase
{
	public:
		Vector ( double x, double y, double z,std::vector<DOFPointer> dof_list );
		Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
	
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
		SketchPlane ( VectorPointer normal, VectorPointer up );

	private:
		VectorPointer normal_;
		VectorPointer up_;
};

// constraint equation class
class ConstraintEquationBase
{
	public:
		virtual ~ConstraintEquationBase() {constraints_.clear(); dof_list_.clear();}

		virtual void Draw() const {;}
	
		// Accessor methods
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}

	protected:
		// constraints and constraint_weights_ are parallel vectors
		// stores constraints
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_; 

		// this vector stores the weights for the constraints
		std::vector< double > constraint_weights_; 

		std::vector<DOFPointer> dof_list_;

};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;

class DistanceConstraint : public ConstraintEquationBase
{
	public:
		DistanceConstraint(const PointPointer point1, const PointPointer point2, double distance);

	private:
};
typedef boost::shared_ptr<DistanceConstraint> DistanceConstraintPointer;

class ParallelConstraint : public ConstraintEquationBase
{
	public:
		ParallelConstraint(const LinePointer line1, const LinePointer line2);

	private:

};
typedef boost::shared_ptr<ParallelConstraint> ParallelConstraintPointer;



#endif //PrimitivesH
