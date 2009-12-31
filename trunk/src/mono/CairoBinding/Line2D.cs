
using System;
using Cairo;
using pSketcher.ConstraintSolver;

namespace pSketcher.CairoBinding
{


    class Line2D : pSketcher.ConstraintSolver.Line2D, IGraphicsItem
    {

        public Line2D (SketchPlane sketch_plane, Point2D point1, Point2D point2) :
            base(sketch_plane, point1, point2)
        {
        }

        public void Display(Cairo.Context context, double unit_screen_width)
        {
            context.Color = new Color(0, 0, 0);
            context.MoveTo(Point1.S.Value, -Point1.T.Value);
            context.LineTo(Point2.S.Value, -Point2.T.Value);
            context.LineWidth = 2.0*unit_screen_width;
            context.Stroke();

            Console.WriteLine("drawing line");
        }


    }
}
