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
		virtual void Draw() const {;}

	private:

};

typedef boost::shared_ptr<PrimitiveBase> PrimitiveBasePointer;


// point class
class Point : public PrimitiveBase
{
	public:
		Point ( double x, double y, double z );
		Point ( DOFPointer x, DOFPointer y, DOFPointer z );

		DOFPointer GetXDOF()const {return x_;}
		DOFPointer GetYDOF()const {return y_;}
		DOFPointer GetZDOF()const {return z_;}

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};

// line class
class Line : public PrimitiveBase
{
	public:
		Line (const Point &point1, const Point &point2);

	private:
		DOFPointer x1_;
		DOFPointer y1_;
		DOFPointer z1_;
		DOFPointer x2_;
		DOFPointer y2_;
		DOFPointer z2_;
};


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
class ConstraintEquation
{
	public:
		void Draw() const {;}

		void SetDisplacement(const Point &point1, const Point &point2, double distance);

	private:
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_;
		std::vector<DOFPointer> dof_list_;

};

typedef boost::shared_ptr<ConstraintEquation> ConstraintEquationPointer;

#endif //PrimitivesH
