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
	DOF(double value = 0.0, bool free = false);
	DOF(const char *name, double value = 0.0, bool free = false);
	~DOF();

	//Accessor methods
	void SetValue(double value) {value_ = value;}
	double GetValue()const {return value_;}

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
	PrimitiveBase();
	virtual ~PrimitiveBase();
	virtual void Draw();

private:

};

typedef boost::shared_ptr<DOF> PrimitiveBasePointer;


// constraint equation class
class ConstraintEquation
{
public:
	ConstraintEquation();
	~ConstraintEquation();

	void Draw();

private:
	vector< boost::shared_ptr<GiNaC::ex> > constraints_;
	
};

typedef boost::shared_ptr<DOF> ConstraintEquationPointer;

// point class
class Point : public PrimitiveBase
{
public:
	Point(double x,double y,double z,vector<DOFPointer> dof_list);
	Point(DOFPointer x, DOFPointer y, DOFPointer z);
	~Point();

private:
	DOFPointer x_;
	DOFPointer y_;
	DOFPointer z_;
};

typedef boost::shared_ptr<DOF> PointPointer;

// line class
class Line : public PrimitiveBase
{
public:
	Line(PointPointer point1, PointPointer point2);
	~Line();

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
	Vector(double x, double y, double z,vector<DOFPointer> dof_list);
	Vector(DOFPointer x, DOFPointer y, DOFPointer z);
	~Vector();

private:
	DOFPointer x_;
	DOFPointer y_;
	DOFPointer z_;
};

typedef boost::shared_ptr<DOF> VectorPointer;

// sketch plane class (includes up vector)
class SketchPlane : public PrimitiveBase
{
public:
	SketchPlane(VectorPointer normal, VectorPointer up);
	~SketchPlane();

private:
	VectorPointer normal_;
	VectorPointer up_;
};


#endif //PrimitivesH
