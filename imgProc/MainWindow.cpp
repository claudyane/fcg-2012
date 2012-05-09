/* 
 * File:   MainWindow.cpp
 * Author: walther
 * 
 * Created on April 10, 2012, 4:43 PM
 */

#include <gtk/gtkfilechooserdialog.h>
#include <gtk/gtkcontainer.h>
#include <string.h>
#include <gtk-2.0/gtk/gtkmenushell.h>


extern "C"
{
    #include <gtk/gtk.h>
    #include <gtk/gtkbutton.h>
    #include <gtk/gtkcombobox.h>
    #include <gtk/gtkgl.h>
    #include <gtk/gtkdrawingarea.h>
}

#include <GL/gl.h>
#include <gtk-2.0/gtk/gtkstyle.h>
#include <gtk-2.0/gtk/gtkfilechooser.h>
#include <gtk-2.0/gtk/gtkdialog.h>
#include <gtk-2.0/gtk/gtkbox.h>
#include <gtk-2.0/gtk/gtkmenuitem.h>
#include <gtk-2.0/gtk/gtkmain.h>
#include <gtk-2.0/gtk/gtkfilefilter.h>
#include <gtk-2.0/gtk/gtkframe.h>
#include <gtk-2.0/gtk/gtkseparatormenuitem.h>
#include "image.h"
#include "MainWindow.h"

#define GRAYSCALE     "Greyscale"
#define GAUSS         "Gauss"
#define MEDIAN        "Median"
#define EDGES         "Edges"
#define BINOTSU       "BinOtsu"
#define BINOHBUCHI    "BinOhbuchi"
#define FOURIER_MAG   "Fourier Magnitude"
#define FOURIER_PHASE "Fourier Phase"
#define INV_FOURIER   "Inverse Fourier"
#define SHIFT         "Shift"
#define GAMMA_UP      "Gamma +"
#define GAMMA_DOWN    "Gamma -"
#define NORMALIZE     "Normalize"
#define LOW_PASS      "Low Pass"
#define HIGH_PASS     "High Pass"
#define BAND_PASS     "Band Pass"

MainWindow::MainWindow()
{
    _presenter = new MainWindowPresenter();
    
    build();
    
    g_signal_connect_swapped( _window, "destroy", G_CALLBACK (gtk_main_quit), NULL );
    g_signal_connect( _dstCanvas    , "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( _dstCanvas    , "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );
    g_signal_connect( _srcCanvas    , "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( _srcCanvas    , "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );
    g_signal_connect( _filterChooser, "changed"        , G_CALLBACK( cb_comboBoxSelectItem ), this );
    
    gtk_widget_show_all( _window );
}



MainWindow::~MainWindow()
{
}

GtkWidget* MainWindow::buildFirstCanvasBox()
{
    GtkWidget* canvasFrame_1 = gtk_frame_new( "Original Image" );
    gtk_frame_set_label_align( GTK_FRAME( canvasFrame_1 ), 0.5f, 1.0f );
    
    gtk_container_add( GTK_CONTAINER( canvasFrame_1 ), _srcCanvas );
    
    return canvasFrame_1;
}

GtkWidget* MainWindow::buildSecondCanvasBox()
{
    GtkWidget* canvasFrame_2 = gtk_frame_new( "Edited Image" );
    gtk_frame_set_label_align( GTK_FRAME( canvasFrame_2 ), 0.5f, 1.0f );
    
    gtk_container_add( GTK_CONTAINER( canvasFrame_2 ), _dstCanvas );
    
    return canvasFrame_2;
}

GtkWidget* MainWindow::buildTwoCanvasBox()
{
    GtkWidget* twoCanvasBox = gtk_hbox_new( false, 1 );
    
    buildCanvas();
    GtkWidget* firstCanvasBox  = buildFirstCanvasBox();
    GtkWidget* secondCanvasBox = buildSecondCanvasBox();
    
    //create and insert drawing area
    gtk_box_pack_start_defaults( GTK_BOX( twoCanvasBox ), firstCanvasBox );
    gtk_box_pack_start_defaults( GTK_BOX( twoCanvasBox ), secondCanvasBox );
    
    return twoCanvasBox;
}

GtkWidget* MainWindow::buildFilterBox()
{
    GtkWidget* filterBox = gtk_hbox_new( false, 5 );
    
    GtkWidget* gap = gtk_label_new( "" );
    gtk_box_pack_start( GTK_BOX( filterBox ), gap, FALSE, FALSE, 5 );
    
    GtkWidget* filterLabel = gtk_label_new( "Filter:" );
    gtk_box_pack_start( GTK_BOX( filterBox ), filterLabel, FALSE, FALSE, 0 );
    
    buildFilterChooser();
    gtk_box_pack_start( GTK_BOX( filterBox ), _filterChooser, FALSE, TRUE, 0 );

    buildButtons();
    gtk_box_pack_start( GTK_BOX( filterBox ), _applyButton, FALSE, FALSE, 0 );
    
    GtkWidget* fill = gtk_label_new( "" );
    gtk_box_pack_start( GTK_BOX( filterBox ), fill, TRUE, TRUE, 0 );
    
    GtkWidget* lowLabel = gtk_label_new( "Lowest:" );
    gtk_box_pack_start( GTK_BOX( filterBox ), lowLabel, FALSE, FALSE, 0 );
    
    _spinLow  = gtk_spin_button_new_with_range( 0.0, 1024.0, 1.0 );
    gtk_widget_set_sensitive( GTK_WIDGET(_spinLow), false );
    gtk_box_pack_start( GTK_BOX( filterBox ), _spinLow, FALSE, FALSE, 0 );
    
    GtkWidget* highLabel = gtk_label_new( "Highest:" );
    gtk_box_pack_start( GTK_BOX( filterBox ), highLabel, FALSE, FALSE, 0 );
    
    _spinHigh = gtk_spin_button_new_with_range( 0.0, 1024.0, 1.0 );
    gtk_widget_set_sensitive( GTK_WIDGET(_spinHigh), false );
    gtk_box_pack_start( GTK_BOX( filterBox ), _spinHigh, FALSE, FALSE, 0 );
    
    return filterBox;
}

GtkWidget* MainWindow::buildMainBox()
{
    GtkWidget* mainBox = gtk_vbox_new( false, 5 );
    
    buildMenu();
    gtk_box_pack_start_defaults( GTK_BOX( mainBox ), _menuBar );
    
    GtkWidget* filterBox = buildFilterBox();
    gtk_box_pack_start( GTK_BOX( mainBox ), filterBox, FALSE, FALSE, 5 );
    
    GtkWidget* separator = gtk_hseparator_new();
    gtk_box_pack_start( GTK_BOX( mainBox ), separator, FALSE, FALSE, 5 );
    
    GtkWidget* twoCanvasBox = buildTwoCanvasBox();
    gtk_box_pack_start_defaults( GTK_BOX( mainBox ), twoCanvasBox );
    
    return mainBox;
}

void MainWindow::build()
{
    //create window
    _window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    
    //create mainBox
    GtkWidget* mainBox = buildMainBox();
    
    //insert mainBox as outter most layout
    gtk_container_add( GTK_CONTAINER( _window ), mainBox );
}



void MainWindow::buildMenu()
{
    // create Menu Bar
    _menuBar = gtk_menu_bar_new();
    
    // File Menu
    GtkWidget* fileMenuItem = gtk_menu_item_new_with_label( "File" );
    gtk_menu_shell_append( GTK_MENU_SHELL( _menuBar ), fileMenuItem );
    
    GtkWidget* fileMenu = gtk_menu_new();
    gtk_menu_item_set_submenu( GTK_MENU_ITEM( fileMenuItem ), fileMenu );
    
    GtkWidget* openItem  = gtk_menu_item_new_with_label("Open...");
    gtk_menu_shell_append( GTK_MENU_SHELL( fileMenu ), openItem );
    g_signal_connect( openItem, "activate", G_CALLBACK( cb_openMenuItem ), this );
    
    GtkWidget* saveItem  = gtk_menu_item_new_with_label("Save...");
    gtk_menu_shell_append( GTK_MENU_SHELL( fileMenu ), saveItem );
    g_signal_connect( saveItem, "activate", G_CALLBACK( cb_saveMenuItem ), this );
    
    GtkWidget* resetItem = gtk_menu_item_new_with_label("Reset");
    gtk_menu_shell_append( GTK_MENU_SHELL( fileMenu ), resetItem );
    g_signal_connect( resetItem, "activate", G_CALLBACK( cb_resetMenuItem ), this );
    
    GtkWidget* quitItem  = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append( GTK_MENU_SHELL( fileMenu ), quitItem );
    g_signal_connect( quitItem, "activate", G_CALLBACK( cb_quitMenuItem ), this );
}



void MainWindow::buildButtons()
{
    _applyButton = gtk_button_new_with_label( "Apply" );
    g_signal_connect( _applyButton, "clicked", G_CALLBACK(cb_applyButton), this );
}



void MainWindow::buildFilterChooser()
{
    _filterChooser = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), GRAYSCALE );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), GAUSS );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), MEDIAN );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), EDGES );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), BINOTSU );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), BINOHBUCHI );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), FOURIER_MAG );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), FOURIER_PHASE );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), INV_FOURIER );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), SHIFT );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), GAMMA_UP );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), GAMMA_DOWN );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), NORMALIZE );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), HIGH_PASS );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), LOW_PASS );
    gtk_combo_box_text_append_text( GTK_COMBO_BOX_TEXT( _filterChooser ), BAND_PASS );
    
}



void MainWindow::buildCanvas()
{
    // create the drawing areas
    _dstCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA( _dstCanvas ), 400, 100 );
    _srcCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA( _srcCanvas ), 400, 100 );
    
    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode (static_cast<GdkGLConfigMode>
                                                       (GDK_GL_MODE_RGB  |
                                                        GDK_GL_MODE_DEPTH|
                                                        GDK_GL_MODE_DOUBLE) );
    
    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( _dstCanvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );
    gtk_widget_set_gl_capability( _srcCanvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );
}


gboolean MainWindow::cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );
    
    // start opengl commands
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
    
    glLoadIdentity();
    glViewport( 0, 0, canvas->allocation.width, canvas->allocation.height );
    glMatrixMode( GL_PROJECTION );
    glOrtho ( 0, canvas->allocation.width, 0, canvas->allocation.height, -10, 10 );
    
    // end opengl commands
    gdk_gl_drawable_gl_end( glDrawable );
}



gboolean MainWindow::cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
    
    //Clear the canvas with black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Recover access to the image
    MainWindow* window = ( MainWindow* )user_data;
    
    Image* image = NULL;
    
    if( canvas == window->_dstCanvas )
    {
        image = ( Image* )window->_presenter->getDstImage();
    }
    else
    {
        image = ( Image* )window->_presenter->getSrcImage();
    }
    
    //If Image loaded OK, start drawing
    if( image )
    {
        glBegin( GL_POINTS );
            int width = imgGetWidth( image );
            int height = imgGetHeight( image );

            for (int w = 0; w < width; ++w)
            {
                for (int h = 0; h < height; ++h)
                {
                    float r,g,b;
                    imgGetPixel3f( image, w, h, &r, &g, &b );
                    glColor3f( r,g,b );
                    glVertex2i( w, h );
                }
            }
        glEnd();
    }
    
    //Update image
    gdk_gl_drawable_swap_buffers (glDrawable);
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
    
    //everything went fine
    return TRUE;
}



void MainWindow::cb_applyButton( GtkMenuItem* item, gpointer user_data )
{
    MainWindow* _this = (MainWindow*) user_data;
    
    const char* filter = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT( _this->_filterChooser ) );
    
    if( !filter ) return;
    
    if( strcmp( filter, GRAYSCALE ) == 0 )
    {
        _this->_presenter->applyGrayscale();
    }
    else if( strcmp( filter, GAUSS ) == 0 )
    {
        _this->_presenter->applyGauss();
    }
    else if( strcmp( filter, MEDIAN ) == 0 )
    {
        _this->_presenter->applyMedian();
    }
    else if( strcmp( filter, EDGES ) == 0 )
    {
        _this->_presenter->applyEdges();
    }
    else if( strcmp( filter, BINOTSU ) == 0 )
    {
        _this->_presenter->applyBinOtsu();
    }
    else if( strcmp( filter, BINOHBUCHI ) == 0 )
    {
        _this->_presenter->applyBinOhbuchi();
    }
    else if( strcmp( filter, FOURIER_MAG ) == 0 )
    {
        _this->_presenter->applyFourrier( false );
    }
    else if( strcmp( filter, FOURIER_PHASE ) == 0 )
    {
        _this->_presenter->applyFourrier( true );
    }
    else if( strcmp( filter, INV_FOURIER ) == 0 )
    {
        _this->_presenter->applyInverseFourier();
    }
    else if( strcmp( filter, SHIFT ) == 0 )
    {
        _this->_presenter->applyShift();
    }
    else if( strcmp( filter, GAMMA_UP ) == 0 )
    {
        _this->_presenter->applyGamma( 0.9f );
    }
    else if( strcmp( filter, GAMMA_DOWN ) == 0 )
    {
        _this->_presenter->applyGamma( 1.0f/0.9f );
    }
    else if( strcmp( filter, NORMALIZE ) == 0 )
    {
        _this->_presenter->applyNormalize();
    }
    else if( strcmp( filter, HIGH_PASS ) == 0 )
    {
        int radius = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON(_this->_spinHigh) );
        _this->_presenter->applyHighPass( radius );
    }
    else if( strcmp( filter, LOW_PASS ) == 0 )
    {
        int radius = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON(_this->_spinLow) );
        _this->_presenter->applyLowPass( radius );
    }
    else if( strcmp( filter, BAND_PASS ) == 0 )
    {
        int radiusH = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON(_this->_spinHigh) );
        int radiusL = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON(_this->_spinLow) );
        _this->_presenter->applyBandPass( radiusL, radiusH );
    }
    
    gtk_widget_queue_draw( _this->_dstCanvas );
}



void MainWindow::cb_openMenuItem( GtkMenuItem* item, gpointer user_data )
{
    MainWindow* _this = (MainWindow*) user_data;
    
    GtkWidget* fileChooser = gtk_file_chooser_dialog_new( "Choose an image",
                                                          GTK_WINDOW( _this->_window ),
                                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                          GTK_STOCK_OPEN, GTK_RESPONSE_OK,
                                                          NULL );
    GtkFileFilter* fileFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern( fileFilter, "*.bmp" );
    gtk_file_filter_add_pattern( fileFilter, "*.BMP" );
    
    gtk_file_chooser_add_filter( GTK_FILE_CHOOSER( fileChooser ),  fileFilter );
    
    if (gtk_dialog_run( GTK_DIALOG( fileChooser ) ) == GTK_RESPONSE_OK)
    {
        char *filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( fileChooser ) );
        _this->_presenter->openImage( filename );
        g_free( filename );  
        
        Image* img = _this->_presenter->getSrcImage();
        
        gtk_drawing_area_size( GTK_DRAWING_AREA(_this->_srcCanvas), imgGetWidth(img), imgGetHeight(img) );
        gtk_drawing_area_size( GTK_DRAWING_AREA(_this->_dstCanvas), imgGetWidth(img), imgGetHeight(img) );
        g_print("Opening image %d x %d\n", imgGetWidth(img), imgGetHeight(img) );
        gtk_window_resize( GTK_WINDOW( _this->_window ), 1, 1 );
    }
    
    gtk_widget_destroy( fileChooser );
}



void MainWindow::cb_saveMenuItem(GtkMenuItem* item, gpointer user_data)
{
    MainWindow* _this = (MainWindow*) user_data;
    
    GtkWidget* fileChooser = gtk_file_chooser_dialog_new( "Choose an image",
                                                          GTK_WINDOW( _this->_window ),
                                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                          GTK_STOCK_SAVE, GTK_RESPONSE_OK,
                                                          NULL );
    
    if (gtk_dialog_run( GTK_DIALOG( fileChooser ) ) == GTK_RESPONSE_OK)
    {
        char *filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( fileChooser ) );
        _this->_presenter->saveImage( filename );
        g_free( filename );
    }
    
    gtk_widget_destroy( fileChooser );
}



void MainWindow::cb_resetMenuItem(GtkMenuItem* item, gpointer user_data)
{
    MainWindow* _this = (MainWindow*) user_data;
    
    _this->_presenter->resetImage();
    
    gtk_widget_queue_draw( _this->_dstCanvas );
}



void MainWindow::cb_quitMenuItem(GtkMenuItem* item, gpointer user_data)
{
    gtk_main_quit();
}



void MainWindow::cb_comboBoxSelectItem(GtkComboBoxText* widget, gpointer user_data)
{
    MainWindow* _this = (MainWindow*) user_data;
    
    gchar* item = gtk_combo_box_text_get_active_text( widget );
    
    if (strcmp( item, BAND_PASS ) == 0)
    {
        gtk_widget_set_sensitive( _this->_spinLow,  true );
        gtk_widget_set_sensitive( _this->_spinHigh, true );
    }
    else if (strcmp( item, HIGH_PASS ) == 0)
    {
        gtk_widget_set_sensitive( _this->_spinLow,  false );
        gtk_widget_set_sensitive( _this->_spinHigh, true  );
    }
    else if (strcmp( item, LOW_PASS )  == 0)
    {
        gtk_widget_set_sensitive( _this->_spinLow,  true  );
        gtk_widget_set_sensitive( _this->_spinHigh, false );
    }
    else
    {
        gtk_widget_set_sensitive( _this->_spinLow,  false );
        gtk_widget_set_sensitive( _this->_spinHigh, false );
    }
}