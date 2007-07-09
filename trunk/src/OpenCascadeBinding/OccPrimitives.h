#ifndef OccPrimitivesH
#define OccPrimitivesH

#include "../ConstraintSolver/Primitives.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Point.hxx>
#include <AIS_Line.hxx>
#include <Geom_CartesianPoint.hxx>

// OccPrimitiveBase class
class OccPrimitiveBase
{
	public:
		OccPrimitiveBase(Handle(AIS_InteractiveContext) ais_context) {ais_context_ = ais_context;}

		virtual ~OccPrimitiveBase() {/* @TODO add code here to delete ais_object_ from ais_context_ */;}

	void Display();

	protected:
		Handle(AIS_InteractiveContext) ais_context_;
		Handle(AIS_InteractiveObject) ais_object_;
};

// point class
class OccPoint : public OccPrimitiveBase
{
	public:
		OccPoint (const PointPointer point, Handle(AIS_InteractiveContext) ais_context);
		OccPoint (const Point2DPointer point, Handle(AIS_InteractiveContext) ais_context);

	private:
		PointPointer point_;

		Handle(Geom_Point) oc_point_;

};
typedef boost::shared_ptr<OccPoint> OccPointPointer;

// point class
class OccPoint2D : public OccPrimitiveBase
{
	public:
		OccPoint2D (const Point2DPointer point, Handle(AIS_InteractiveContext) ais_context);

	private:
		Point2DPointer point_;

		Handle(Geom_Point) oc_point_;

};
typedef boost::shared_ptr<OccPoint2D> OccPoint2DPointer;

// line class
class OccLine : public OccPrimitiveBase
{
	public:
		OccLine (const LinePointer line, Handle(AIS_InteractiveContext) ais_context);

	private:
		LinePointer line_;

		Handle(Geom_Point) oc_point1_;
		Handle(Geom_Point) oc_point2_;
};
typedef boost::shared_ptr<OccLine> OccLinePointer;

// line class
class OccLine2D : public OccPrimitiveBase
{
	public:
		OccLine2D (const Line2DPointer line, Handle(AIS_InteractiveContext) ais_context);

	private:
		Line2DPointer line_;

		Handle(Geom_Point) oc_point1_;
		Handle(Geom_Point) oc_point2_;
};
typedef boost::shared_ptr<OccLine2D> OccLine2DPointer;

#endif //OccPrimitivesH
