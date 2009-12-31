
using System;
using Cairo;

namespace pSketcher.CairoBinding
{
    interface IGraphicsItem
    {
        void Display(Cairo.Context context, double unit_screen_width);
        //bool UnderPoint(Cairo.Context context, double unit_screen_width, double x, double y);

    }
}
