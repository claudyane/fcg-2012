#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdk.h>
#include <gtk/gtkgl.h>
#include "HistogramWindow.h"


HistogramWindow::HistogramWindow()
{   
    _width = 512;
    _height = 512;
    
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
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_container_set_border_width( GTK_CONTAINER (window), 5 );
    gtk_window_set_title( GTK_WINDOW (window), "Histograma");
    gtk_window_set_resizable( GTK_WINDOW (window), FALSE ); 
    g_signal_connect( window, "delete-event", G_CALLBACK (cb_deleteWindow), this );
    
    GtkWidget* canvasBox = gtk_vbox_new( FALSE, 2 );   
    _canvas = buildCanvas();
    gtk_box_pack_start( GTK_BOX(canvasBox), _canvas, FALSE, FALSE, 2 );
    
    gtk_container_add( GTK_CONTAINER(window), canvasBox );           
    
    return window;
}



GtkWidget* HistogramWindow::buildCanvas()
{
    // Create the canvas and set  it's size
    GtkWidget* canvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(canvas), _width, _height );

    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode( static_cast<GdkGLConfigMode>( GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE ) );

    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( canvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );

    // Connect the signals to the callbacks
    g_signal_connect( canvas, "configure-event", G_CALLBACK( cb_configGLCanvas )    , this );
    g_signal_connect( canvas, "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );

    return canvas;
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
    
    //TODO
    
     // Altera a Área de desenho no canvas para apenas uma pequena região no canto superior esquerdo.
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glViewport( 0, 0, window->_width, window->_height );

    // Altera o sistema de coordenadas do desenho que mapear de 0,0 ate largura,atura da regiÃ£o sendo desenhada
    // isto facilita as contas feitas pelo render de cada uma dar partes do desenho.
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, window->_width, 0, window->_height );
    
    glBegin( GL_TRIANGLES );
    
    glColor3f( 1.0f, 0.0f, 0.0f );
    glVertex2d( 0, 0 );
    glVertex2d( window->_width/2, window->_height);
    glVertex2d( window->_width, 0 );
    
    glEnd();
    
    
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

