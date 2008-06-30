#ifndef QtPrimitiveBaseH
#define QtPrimitiveBaseH

#include "DisplayProperties.h"

#include <QGraphicsItem>
#include <QRectF>
#include "../ConstraintSolver/Primitives.h"

// QtPrimitiveBase class
class QtPrimitiveBase : public QGraphicsItem
{
	public:
		QtPrimitiveBase(QGraphicsItem * parent = 0);
		
		// erase from the AIS_InteractiveContext and clear vector container
		virtual ~QtPrimitiveBase() { Erase();}

		virtual void Display();

		void Erase();

		DisplayProperties GetProperties()           {return properties_;}
		DisplayProperties GetSelectedProperties()   {return selected_properties_;}
		DisplayProperties GetMouseHoverProperties() {return mouse_hover_properties_;}
	
		void SetProperties(DefaultPropertiesSet prop_set) {properties_ = DisplayProperties(prop_set);}
		void SetSelectedProperties(DefaultPropertiesSet prop_set) {selected_properties_ = DisplayProperties(prop_set);}
		void SetMouseHoverProperties(DefaultPropertiesSet prop_set) {mouse_hover_properties_ = DisplayProperties(prop_set);}

		// update coordinates of primitive
		virtual void UpdateDisplay();

		virtual bool IsSelected();
		virtual void SetSelectable(bool selectable_);

		// some utility methods used to paint primitives
		void PaintPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y);
		QPolygonF GetArrowPolygon(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, bool double_arrow = false) const;
		QPainterPath GetArcArrowPath(double x_center, double y_center, double radius, double theta1, double theta2, double arrow_head_length, double arrow_head_width) const;

	protected:
		// member class that stores display properties for the primitives
		DisplayProperties properties_;
		DisplayProperties selected_properties_;
		DisplayProperties mouse_hover_properties_;
};


#endif //QtPrimitiveBaseH
