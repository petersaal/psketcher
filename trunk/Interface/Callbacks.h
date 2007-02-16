#include <gtk/gtk.h>
#include <glade/glade.h>

#include "OccGtkGLView.h"

static OccGtkGLView* theViewer = new OccGtkGLView();
static GdkGLConfig *glconfig = 0;

extern "C" {
	
gboolean
on_MainWindow_configure_event          (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

void
on_MainWindow_realize                  (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_MainWindow_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_MainWindow_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_import_iges1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_import_step1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_import_brep1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_import_csfdb1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_iges2_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_step2_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_brep2_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_csfdb2_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_stl1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_top_view1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bottom_view1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_front_view1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_back_view1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_left_view1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_right_view1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_axonometric_view1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_reset_view1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_zoom_view1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_pan_view1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_rotate_view1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_zoom_window1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_fit_all1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_import_iges_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_import_step_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_import_brep_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_import_csfdb_toolbutton_clicked     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_iges_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_step_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_brep_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_csfdb_toolbutton_clicked     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_stl_toolbutton_clicked       (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_about_toolbutton_clicked            (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_top_view_toolbutton_clicked         (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_bottom_view_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_front_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_back_view_toolbutton_clicked        (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_left_view_toolbutton_clicked        (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_right_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_axo_view_toolbutton_clicked         (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_reset_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
                                        gpointer         user_data);


void
on_fit_all_toolbutton_clicked          (GtkToolButton   *toolbutton,
                                        gpointer         user_data);
void
on_dyn_zoom_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_dyn_pan_toolbutton_toggled          (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_dyn_rotate_toolbutton_toggled       (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_glob_pan_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_zoom_win_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_antialiasing_toolbutton_clicked     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_color_toolbutton_clicked            (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_material_toolbutton_clicked         (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

void
on_drawingarea1_realize                (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_button_release_event   (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_motion_notify_event    (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_key_press_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_key_release_event      (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_enter_notify_event     (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_leave_notify_event     (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_destroy_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_drawingarea1_map_event              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_popup_change_background_color1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_wireframe1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_shading1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_color1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_material1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_brass1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bronze1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_copper1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_gold1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_pewter1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_plaster1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_plastic1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_silver1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_transparency1_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_popup_delete1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_transparency_spinbutton_value_changed
                                        (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);
void
on_hlr_toolbutton_toggled              (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_display_mode_toolbutton_toggled     (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_antialiasing_toolbutton_toggled     (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);


void
on_snapshot_toolbutton_clicked         (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void 
set_material                           (int theMaterial);

void
on_export_vrml2_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_image2_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_export_vrml_toolbutton_clicked      (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_export_image_toolbutton_clicked     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_brass_radiobutton_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_bronze_radiobutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_copper_radiobutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_gold_radiobutton_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_pewter_radiobutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_plaster_radiobutton_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_plastic_radiobutton_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_silver_radiobutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_transparency_toolbutton_clicked     (GtkToolButton   *toolbutton,
                                        gpointer         user_data);


gboolean
on_drawingarea1_scroll_event           (GtkWidget       *widget,
                                        GdkEventScroll        *event,
                                        gpointer         user_data);
										
}
