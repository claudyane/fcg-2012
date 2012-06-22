#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdk.h>
#include <gtk/gtkgl.h>
#include "HistogramWindow.h"

#include <iostream>


HistogramWindow::HistogramWindow()
{   
    _width = 1100;
    _height = 550;
    
    _histogram = new Histogram( _width, _height );
    _window = build();
}



HistogramWindow::~HistogramWindow( )
{
    if (_histogram)
        delete _histogram;
}



void HistogramWindow::show()
{
    gtk_widget_show_all( _window );
}
    


void HistogramWindow::hide()
{
    gtk_widget_hide_all( _window );
}



void HistogramWindow::setVolume( Volume* volume )
{
    _histogram->setVolume( volume );
}


GtkWidget* HistogramWindow::build()
{
    // creates window
    _window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_container_set_border_width( GTK_CONTAINER (_window), 5 );
    gtk_window_set_title( GTK_WINDOW (_window), "Histogram");
    gtk_window_set_resizable( GTK_WINDOW (_window), FALSE ); 
    g_signal_connect( _window, "delete-event", G_CALLBACK (cb_deleteWindow), this );
    
    // creates canvas
    GtkWidget* mainBox = gtk_vbox_new( FALSE, 2 );   
    _canvas = buildCanvas();
    gtk_box_pack_start( GTK_BOX(mainBox), _canvas, FALSE, FALSE, 2 );
    
    // separator
    gtk_box_pack_start(GTK_BOX (mainBox), gtk_hseparator_new(), TRUE , TRUE , 2  );    
    
    // creates message bar
    GtkWidget* messageHBox = gtk_hbox_new( FALSE, 2 );
    gtk_box_pack_start( GTK_BOX(mainBox), messageHBox, FALSE, FALSE, 2 );
    _messageBar = gtk_label_new( "Created by Eliana Goldner and Walther Maciel" );
    gtk_box_pack_start( GTK_BOX(messageHBox), _messageBar, FALSE, FALSE, 2 );
    
    // fill
    GtkWidget* fill = gtk_label_new("");
    gtk_box_pack_start( GTK_BOX(messageHBox), fill, TRUE, TRUE, 2 );
    
    gtk_container_add( GTK_CONTAINER(_window), mainBox );           
    
    return _window;
}



GtkWidget* HistogramWindow::buildCanvas()
{
    // Create the canvas and set  it's size
    _canvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(_canvas), _width, _height );
    gtk_widget_add_events( _canvas, (GDK_POINTER_MOTION_MASK) );

    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode( static_cast<GdkGLConfigMode>( GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE ) );

    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( _canvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );

    // Connect the signals to the callbacks
    g_signal_connect( _canvas, "configure-event"     , G_CALLBACK( cb_configGLCanvas ), this );
    g_signal_connect( _canvas, "expose-event"        , G_CALLBACK( cb_exposeGLCanvas ), this );
    g_signal_connect( _canvas, "motion-notify-event", G_CALLBACK(cb_motion)           , this );
    //gtk_signal_connect( GTK_OBJECT (_canvas), "motion-notify-event", GTK_SIGNAL_FUNC(HistogramWindow::cb_motion), this );

    return _canvas;
}



void HistogramWindow::beginGL()
{
    GdkGLContext* glContext = gtk_widget_get_gl_context( _canvas );
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
}
    


void HistogramWindow::endGL()
{
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( _canvas );        
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
}



gboolean HistogramWindow::cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data )
{
    GdkGLDrawable* glDrawable = gtk_widget_get_gl_drawable( canvas );    
    HistogramWindow* window = ( HistogramWindow* )user_data;
    
    window->beginGL();
    
    window->_histogram->draw();    
    
    gdk_gl_drawable_swap_buffers( glDrawable );
    
    window->endGL();
    
    return TRUE;
}



gboolean HistogramWindow::cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data )
{
    HistogramWindow* window = ( HistogramWindow* )user_data;
    
    // begin opengl commands
    window->beginGL();

    glLoadIdentity();
    glViewport( 0, 0, canvas->allocation.width, canvas->allocation.height );

    // end opengl commands
    window->endGL();

    return true;
}



gboolean HistogramWindow::cb_deleteWindow( GtkWidget* widget, GdkEvent* event, gpointer data )
{
    HistogramWindow* window = ( HistogramWindow* )data;
    window->hide();
    
    return TRUE;
}



gboolean HistogramWindow::cb_motion( GtkWidget* widget, GdkEvent* event, gpointer user_data )
{  
    HistogramWindow* window = (HistogramWindow*)user_data;
    GdkEventMotion* motionEvent = (GdkEventMotion*) event;

    int x = (int)motionEvent->x;
    
    gtk_label_set_label( GTK_LABEL(window->_messageBar), window->_histogram->getInfo( x ).c_str() );
    
    gtk_widget_queue_draw( window->_messageBar );

    return TRUE;
}

