#ifndef PrimitivesH
#define PrimitivesH

#include <vector>
#include "../mmcMatrixClass/mmcMatrix.h"
#include <ginac/ginac.h>

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
	symbol variable_;
	bool free_;
	double value_;
	int reference_counter_;
};

class DOFSet
{
public:


private:
    vector<*DOF> dof_list;
};

/* Now will define the merit function derived class used in the template matching */
class Primitive
{
public:
	virtual PrimitiveBase();
	virtual ~PrimitiveBase();
	virtual void Draw();

private:

};

// line class
class Line : public Primitive
{
public:
	Line(Point *point1, Point *point2);
	~Line();

private:
	DOF *x1_;
	DOF *y1_;
	DOF *z1_;
	DOF *x2_;
	DOF *y2_;
	DOF *z2_;
};

// point class
class Point : public Primitive
{
public:
	Point(double x,double y,double z,vector<*DOF> dof_list);
	Point(DOF *x, DOF *y, DOF *z);
	~Point();

private:
	DOF *x_;
	DOF *y_;
	DOF *z_;
};

// vector class
class Vector : public Primitive
{
public:
	Vector(double x, double y, double z,vector<*DOF> list);
	Vector(DOF *x, DOF *y, DOF *z);
	~Vector();

private:
	DOF *x_;
	DOF *y_;
	DOF *z_;
};

// sketch plane class (includes up vector)
class SketchPlane : public Primitive
{
public:
	SketchPlane(Vector *normal, Vector *up);
	~SletchPlane();

private:
	Vector *normal_;
	Vector *up_;
};


// constraint equation class
class ConstraintEquation
{
public:
	ConstraintEquation();
	~ConstraintEquation();

private:

};



#endif //PrimitivesH
