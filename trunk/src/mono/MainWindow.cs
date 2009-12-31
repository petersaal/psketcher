using System;
using Gtk;
using pSketcherCore = pSketcher.ConstraintSolver;
using pSketcher.CairoBinding;
using pSketcher.ConstraintSolver;

public partial class MainWindow : Gtk.Window
{

    //
    // Field data
    //
    private pSketcherCore.Model model_ = new pSketcherCore.Model();
    private ModelViewer model_viewer_ = null;

    public MainWindow () : base(Gtk.WindowType.Toplevel)
    {
        model_viewer_ = new ModelViewer(model_);

        Build ();

        this.vbox2.Add(model_viewer_);
        model_viewer_.Show();
    }

    protected void OnDeleteEvent (object sender, DeleteEventArgs a)
    {
        Application.Quit ();
        a.RetVal = true;
    }

    protected virtual void OnExecuteActionActivated (object sender, System.EventArgs e)
    {
        var normal = new Vector3D (0.0,0.0,1.0);
        var up = new Vector3D (0.0,1.0,0.0);
        var @base = new Point (0.0,0.0,0.0);
        var sketch_plane = new SketchPlane(normal,up,@base);

        for(int i = 0; i < 1; i++) {
            var point1 = new Point2D(sketch_plane,0.0,0.0,false,false);
            var point2 = new Point2D(sketch_plane,10.0,0.0,true,false);
            var point3 = new Point2D(sketch_plane,10.0,10.0,true,true);

            var arc1 = new Arc2D(sketch_plane,1.5,6.0,(Math.PI/2.0)*0.8,Math.PI*1.2,2.0,true,true,true,true,false);

            var line1 = new pSketcher.CairoBinding.Line2D(sketch_plane,point1,point2);
            var line2 = new pSketcher.CairoBinding.Line2D(sketch_plane,point2,point3);
            var line3 = new pSketcher.CairoBinding.Line2D(sketch_plane,point3,arc1.Point1);
            var line4 = new pSketcher.CairoBinding.Line2D(sketch_plane,arc1.Point2,point1);

            var constraint1 = new DistancePoint2D(point1,point2,6.0);
            var constraint2 = new DistancePoint2D(point2,point3,12.0);
            var constraint3 = new ParallelLine2D(line1,line3);
            var constraint4 = new ParallelLine2D(line2,line4);
            var constraint5 = new AngleLine2D(line1,line2,Math.PI*0.5,false);
            var constraint6 = new TangentEdge2D(line3, EdgePointNumber.Point2,
                                                arc1, EdgePointNumber.Point1);
            var constraint7 = new TangentEdge2D(line4, EdgePointNumber.Point1,
                                                arc1, EdgePointNumber.Point2);

            model_.AddPrimitives(point1,point2,point3,arc1,line1,line2,line3,line4);
            model_.AddConstraints(constraint1,constraint2,constraint3,constraint4,constraint5,constraint6,constraint7);

        }

        model_.SolveConstraints();
    }

}
