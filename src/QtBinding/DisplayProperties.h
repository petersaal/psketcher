#ifndef DisplayPropertiesH
#define DisplayPropertiesH

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QString>

const QString LineEditStyleSheet("QLineEdit { border-width: 2px; border-style: solid; border-color: rgb(166,86,0);}");

enum DefaultPropertiesSet {Primitive, SelectedPrimitive, HoverPrimitive, Annotation, SelectedAnnotation, HoverAnnotation, PointPrimitive, SelectedPointPrimitive, HoverPointPrimitive};

class DisplayProperties
{
	public:
		DisplayProperties(DefaultPropertiesSet prop_set = Primitive);

		// annotation properties
		double GetLeaderGap() 			{return leader_gap_;}
		double GetLeaderExtension() 	{return leader_extension_;}
		double GetArrowHeadLength()		{return arrow_head_length_;}
		double GetArrowHeadWidth()		{return arrow_head_width_;}
		double GetZ() 					{return z_;}
		QColor GetPenColor() 			{return pen_color_;}
		double GetPenWidth() 			{return pen_width_;}
		QPen   GetPen(double scale=1.0)	{return QPen(pen_color_, pen_width_/scale);}
		QBrush GetBrush() 				{return brush_;}


	private:
		// annotation properties
		double leader_gap_;
		double leader_extension_;
		double arrow_head_length_;
		double arrow_head_width_;
		double z_;
		QColor pen_color_;
		double pen_width_;
		QBrush brush_;
};




#endif //DisplayPropertiesH
