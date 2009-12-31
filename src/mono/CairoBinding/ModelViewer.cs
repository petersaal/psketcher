
using System;
//using Cairo;
using Gtk;
using pSketcher.ConstraintSolver;

namespace pSketcher.CairoBinding
{

    class MousePanProperties
    {
        public double MouseX { private set; get; }
        public double MouseY { private set; get; }
        public double PreviousX { private set; get; }
        public double PreviousY { private set; get; }

        public MousePanProperties (double mouseX, double mouseY, double previousX, double previousY)
        {
            this.MouseX = mouseX;
            this.MouseY = mouseY;
            this.PreviousX = previousX;
            this.PreviousY = previousY;
        }
    }

    class ModelViewer : DrawingArea
    {
        //
        // Properties and field data
        //
        public Model Model { get; private set; }
        public double Scale { get; set; }
        public double XTranslation { get; set; }
        public double YTranslation { get; set; }
        private MousePanProperties previous_button_2_click_properties_ = null;

        //
        // Constructors
        //
        public ModelViewer (Model model)
        {
            Scale = 1.0;
            XTranslation = 0.0;
            YTranslation = 0.0;

            this.Model = model;

            AddEvents((int)Gdk.EventMask.AllEventsMask);
        }

        //
        // Methods
        //

        protected override bool OnExposeEvent (Gdk.EventExpose args)
        {
            using (Cairo.Context context = Gdk.CairoHelper.Create (GdkWindow)){
                // perform transforms
                context.Translate(XTranslation,YTranslation);
                context.Scale(Scale,Scale);

                // paint the background
                context.SetSourceRGB(1.0,1.0,1.0);
                context.Paint();

                context.SetSourceRGB(0.0,0.0,0.0);

                double ux=1, uy=1;
                context.InverseTransformDistance(ref ux, ref uy);
                double unit_screen_width = (ux+uy)*0.5;

                // Loop through all of the primitives and constraints and display
                // those that implement IGraphicsItem.
                foreach( Primitive current_primitive in Model.PrimitiveDictionary.Values ) {
                    if (current_primitive is IGraphicsItem)
                        ((IGraphicsItem) current_primitive).Display(context,unit_screen_width);
                }
                foreach( Primitive current_primitive in Model.ConstraintEquationDictionary.Values ) {
                    if (current_primitive is IGraphicsItem)
                        ((IGraphicsItem) current_primitive).Display(context,unit_screen_width);
                }
            }

            return true;
        }

        protected override bool OnScrollEvent (Gdk.EventScroll args)
        {
            // Moving the mouse wheel up or down causes the drawing area to zoom in and out

            int numSteps;

            if(args.Direction == Gdk.ScrollDirection.Up)
                numSteps = 1;
            else if(args.Direction == Gdk.ScrollDirection.Down)
                numSteps = -1;
            else
                numSteps = 0;

            if(numSteps != 0)
            {
                double factor = Math.Pow(1.125, -numSteps);

                // The following code centers the zoom about the current mouse location
                using (Cairo.Context context = Gdk.CairoHelper.Create (GdkWindow)){
                    double scene_x = args.X;
                    double scene_y = args.Y;

                    context.Translate(XTranslation,YTranslation);
                    context.Scale(Scale,Scale);

                    context.InverseTransformPoint(ref scene_x, ref scene_y);

                    XTranslation += -Scale*factor*scene_x + Scale*scene_x;
                    YTranslation += -Scale*factor*scene_y + Scale*scene_y;
                }

                Scale *= factor;

                QueueDraw();

                Console.WriteLine("scale = {0}", Scale);
            }

            return true;
        }

        protected override bool OnMotionNotifyEvent (Gdk.EventMotion args)
        {
            // if the middle mouse button is dragged, scroll the drawing area
            if ((args.State & Gdk.ModifierType.Button2Mask) != 0) {
                if (previous_button_2_click_properties_ != null) {
                    XTranslation = (args.X - previous_button_2_click_properties_.MouseX) + previous_button_2_click_properties_.PreviousX;
                    YTranslation = (args.Y - previous_button_2_click_properties_.MouseY) + previous_button_2_click_properties_.PreviousY;

                    QueueDraw();

                    Console.WriteLine ("delta x={0}, delta y={1}",XTranslation,YTranslation);
                }
            }

            return true;
        }

        protected override bool OnButtonPressEvent (Gdk.EventButton args)
        {
            // If the middle moused button is clicked, store the event args so that
            // drag properties can be calculated for the MotionNotifyEvent
            if (args.Button == 2) {
                previous_button_2_click_properties_ = new MousePanProperties(args.X, args.Y, XTranslation, YTranslation);
                Console.WriteLine("button click");
            }

            return true;
        }


    }
}
