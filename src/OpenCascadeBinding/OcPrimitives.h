#ifndef OcPrimitivesH
#define OcPrimitivesH

#include "../ConstraintSolver/Primitives.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Point.hxx>
#include <AIS_Line.hxx>
#include <Geom_CartesianPoint.hxx>

// OcPrimitiveBase class
class OcPrimitiveBase
{
	public:
		OcPrimitiveBase(Handle(AIS_InteractiveContext) ais_context) {ais_context_ = ais_context;}

		virtual ~OcPrimitiveBase() {/* @TODO add code here to delete ais_object_ from ais_context_ */;}

	void Display();

	protected:
		Handle(AIS_InteractiveContext) ais_context_;
		Handle(AIS_InteractiveObject) ais_object_;
};

// point class
class OcPoint : public OcPrimitiveBase
{
	

	public:
		OcPoint (const PointPointer point, Handle(AIS_InteractiveContext) ais_context);

	private:
		PointPointer point_;

		Handle(Geom_Point) oc_point_;

};
typedef boost::shared_ptr<OcPoint> OcPointPointer;

// line class
class OcLine : public OcPrimitiveBase
{
	public:
		OcLine (const LinePointer line, Handle(AIS_InteractiveContext) ais_context);

	private:
		LinePointer line_;

		Handle(Geom_Point) oc_point1_;
		Handle(Geom_Point) oc_point2_;
};
typedef boost::shared_ptr<OcLine> OcLinePointer;

#endif //OcPrimitivesH
